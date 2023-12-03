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

std::string dayOfWeek(fwoop::DateTime dt, int ahead = 0)
{
    dt.addDays(ahead);
    switch (dt.dayOfWeek()) {
    case fwoop::DateTime::Sunday:
        return "Sun";
    case fwoop::DateTime::Monday:
        return "Mon";
    case fwoop::DateTime::Tuesday:
        return "Tue";
    case fwoop::DateTime::Wednesday:
        return "Wed";
    case fwoop::DateTime::Thursday:
        return "Thu";
    case fwoop::DateTime::Friday:
        return "Fri";
    case fwoop::DateTime::Saturday:
        return "Sat";
    default:
        return "N/A";
    }
}

std::string getFormattedTime(const fwoop::DateTime &t)
{
    std::string res = dayOfWeek(t);
    res += " ";
    res += (t.hour() < 10 ? "0" : "") + std::to_string(t.hour());
    res += ":";
    res += (t.minute() < 10 ? "0" : "") + std::to_string(t.minute());
    return res;
}

std::string getFormattedTemp(double temp) { return std::to_string(int(std::round(temp))) + DEGREE_FAHRENHEIT; }

std::string getFormattedPercent(double num) { return std::to_string(int(std::round(num))) + "%"; }

} // namespace

Chestnut::Chestnut()
    : d_publisher(std::make_shared<OpenMetricsPublisher>(Config::metricsPort())),
      d_weatherClient(std::make_shared<WeatherClient>(Config::weatherBaseURL(), Config::weatherLat(),
                                                      Config::weatherLon(), Config::weatherAPIKey())),
      d_httpServer(Config::httpPort(), fwoop::HttpVersion::Http1_1, 50), d_needStop(false)
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
            auto now = fwoop::DateTime::now();
            // update every 60 seconds
            if (t % 60 == 0) {
                fwoop::Log::Debug("server side event update");
                auto data = d_weatherClient->data();
                auto today = data.getDaily(0);
                if (!serverEvent.pushEvent("today-temp", getFormattedTemp(today.getTemperature())) ||
                    !serverEvent.pushEvent("today-max", getFormattedTemp(today.getMaxTemp())) ||
                    !serverEvent.pushEvent("today-min", getFormattedTemp(today.getMinTemp())) ||
                    !serverEvent.pushEvent("today-humidity", std::to_string(today.getHumidity()) + "%") ||
                    !serverEvent.pushEvent("today-precip", getFormattedPercent(today.getPrecipChance() * 100)) ||
                    !serverEvent.pushEvent("forecast-plus-1", dayOfWeek(now, 1)) ||
                    !serverEvent.pushEvent("forecast-plus-1-temp-max",
                                           getFormattedTemp(data.getDaily(1).getMaxTemp())) ||
                    !serverEvent.pushEvent("forecast-plus-1-temp-min",
                                           getFormattedTemp(data.getDaily(1).getMinTemp())) ||
                    !serverEvent.pushEvent("forecast-plus-1-precip",
                                           getFormattedPercent(data.getDaily(1).getPrecipChance() * 100)) ||
                    !serverEvent.pushEvent("forecast-plus-2", dayOfWeek(now, 2)) ||
                    !serverEvent.pushEvent("forecast-plus-2-temp-max",
                                           getFormattedTemp(data.getDaily(2).getMaxTemp())) ||
                    !serverEvent.pushEvent("forecast-plus-2-temp-min",
                                           getFormattedTemp(data.getDaily(2).getMinTemp())) ||
                    !serverEvent.pushEvent("forecast-plus-2-precip",
                                           getFormattedPercent(data.getDaily(2).getPrecipChance() * 100)) ||
                    !serverEvent.pushEvent("forecast-plus-3", dayOfWeek(now, 3)) ||
                    !serverEvent.pushEvent("forecast-plus-3-temp-max",
                                           getFormattedTemp(data.getDaily(3).getMaxTemp())) ||
                    !serverEvent.pushEvent("forecast-plus-3-temp-min",
                                           getFormattedTemp(data.getDaily(3).getMinTemp())) ||
                    !serverEvent.pushEvent("forecast-plus-3-precip",
                                           getFormattedPercent(data.getDaily(3).getPrecipChance() * 100)) ||
                    !serverEvent.pushEvent("forecast-plus-4", dayOfWeek(now, 4)) ||
                    !serverEvent.pushEvent("forecast-plus-4-temp-max",
                                           getFormattedTemp(data.getDaily(4).getMaxTemp())) ||
                    !serverEvent.pushEvent("forecast-plus-4-temp-min",
                                           getFormattedTemp(data.getDaily(4).getMinTemp())) ||
                    !serverEvent.pushEvent("forecast-plus-4-precip",
                                           getFormattedPercent(data.getDaily(4).getPrecipChance() * 100)) ||
                    !serverEvent.pushEvent("forecast-plus-5", dayOfWeek(now, 5)) ||
                    !serverEvent.pushEvent("forecast-plus-5-temp-max",
                                           getFormattedTemp(data.getDaily(5).getMaxTemp())) ||
                    !serverEvent.pushEvent("forecast-plus-5-temp-min",
                                           getFormattedTemp(data.getDaily(5).getMinTemp())) ||
                    !serverEvent.pushEvent("forecast-plus-5-precip",
                                           getFormattedPercent(data.getDaily(5).getPrecipChance() * 100))) {
                    break;
                }
                t = 0;
            }
            t++;
            // always update the time
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
