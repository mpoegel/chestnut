#include <chestnut.h>

#include <config.h>
#include <publisher.h>

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <thread>

#include <fwoop_datetime.h>
#include <fwoop_httpserver.h>
#include <fwoop_httpversion.h>
#include <fwoop_log.h>

namespace chestnut {
namespace {
const std::string DEGREE_FAHRENHEIT = "&#x2109;";

std::string getFormattedTime(const fwoop::DateTime &t)
{
    std::string res;
    res += (t.hour() < 10 ? "0" : "") + std::to_string(t.hour());
    res += ":";
    res += (t.minute() < 10 ? "0" : "") + std::to_string(t.minute());
    return res;
}

} // namespace

Chestnut::Chestnut()
    : d_publisher(std::make_shared<OpenMetricsPublisher>(Config::metricsPort())),
      d_weatherClient(std::make_shared<WeatherClient>(Config::weatherBaseURL(), Config::weatherLat(),
                                                      Config::weatherLon(), Config::weatherAPIKey())),
      d_httpServer(Config::httpPort(), fwoop::HttpVersion::Http1_1), d_needStop(false)
{
}

Chestnut::Chestnut(std::shared_ptr<Publisher> publisher)
    : d_publisher(publisher), d_httpServer(Config::httpPort(), fwoop::HttpVersion::Http1_1), d_needStop(false)
{
}

void Chestnut::refresh()
{
    while (!d_needStop) {
        if (Config::isWeatherEnabled()) {
            d_weatherClient->refresh();
        }
        std::this_thread::sleep_for(std::chrono::seconds(Config::refreshInterval()));
    }
}

void Chestnut::serveHttp()
{
    d_httpServer.addStaticRoute("/", Config::httpStaticDir() + "/index.html");
    d_httpServer.addStaticRoute("/static/index.css", Config::httpStaticDir() + "/index.css");

    fwoop::HttpServerEventHandlerFunc_t eventHandler = [&](const fwoop::HttpRequest &request,
                                                           fwoop::HttpServerEvent &serverEvent) {
        unsigned int t = 60;
        while (!d_needStop) {
            // update every 60 seconds
            if (t % 60 == 0) {
                fwoop::Log::Debug("server side event update");
                auto data = d_weatherClient->data();
                auto today = data.getDaily(0);
                if (!serverEvent.pushEvent("today-temp", std::to_string(int(std::round(today.getTemperature()))) +
                                                             DEGREE_FAHRENHEIT) ||
                    !serverEvent.pushEvent("today-max",
                                           std::to_string(int(std::round(today.getMaxTemp()))) + DEGREE_FAHRENHEIT) ||
                    !serverEvent.pushEvent("today-min",
                                           std::to_string(int(std::round(today.getMinTemp()))) + DEGREE_FAHRENHEIT) ||
                    !serverEvent.pushEvent("today-humidity", std::to_string(today.getHumidity()) + "%") ||
                    !serverEvent.pushEvent("today-precip",
                                           std::to_string(int(std::round(today.getPrecipChance() * 100))) + "%")) {
                    break;
                }
                t = 0;
            }
            t++;
            // always update the time
            auto now = fwoop::DateTime::now();
            now.addHours(Config::timezone());
            if (!serverEvent.pushEvent("timestamp", getFormattedTime(now))) {
                return;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };
    d_httpServer.addServerEventRoute("/updates", eventHandler);

    int rc = d_httpServer.serve();
    if (0 != rc) {
        fwoop::Log::Error("http server failure, rc=", rc);
    }
}

int Chestnut::start()
{
    if (!Config::debug()) {
        fwoop::Log::SetThreshold(fwoop::Log::e_Info);
    }

    d_publisher->start();
    d_weatherClient->setPublisher(d_publisher);
    d_httpThread = std::thread(std::bind(&Chestnut::serveHttp, this));
    refresh();
    return 0;
}

void Chestnut::stop()
{
    d_needStop = true;
    d_publisher->stop();
    d_httpServer.stop();
    d_refreshThread.join();
    d_httpThread.join();
}

} // namespace chestnut
