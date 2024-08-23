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

std::string getFormattedTime(const fwoop::DateTime &t)
{
    std::string res = t.dayOfWeekShortString();
    res += " ";
    res += (t.hour() < 10 ? "0" : "") + std::to_string(t.hour());
    res += ":";
    res += (t.minute() < 10 ? "0" : "") + std::to_string(t.minute());
    return res;
}

} // namespace

Chestnut::Chestnut()
    : d_publisher(std::make_shared<OpenMetricsPublisher>(Config::metricsPort())),
      d_weatherClient(std::make_shared<WeatherClient>(Config::weatherBaseURL(), Config::weatherLat(),
                                                      Config::weatherLon(), Config::weatherAPIKey(),
                                                      Config::timezone())),
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
            fwoop::Log::Info("weather refresh started");
            d_weatherClient->refresh(this);
            fwoop::Log::Info("weather refresh finished");
        }

        // TODO push timestamp every second
        auto now = fwoop::DateTime::now();
        now.addHours(Config::timezone());
        pushEvent("timestamp", getFormattedTime(now));

        std::this_thread::sleep_for(std::chrono::seconds(Config::refreshInterval()));
    }
    fwoop::Log::Info("chestnut refresh stopped");
}

void Chestnut::serveHttp()
{
    d_httpServer.addStaticRoute("/", Config::httpStaticDir() + "/index.html");
    d_httpServer.addStaticRoute("/static/index.css", Config::httpStaticDir() + "/index.css");

    fwoop::HttpServerEventHandlerFunc_t eventHandler = [&](const fwoop::HttpRequest &request,
                                                           fwoop::HttpServerEvent &serverEvent) {
        Connection conn(request, serverEvent);
        d_activeConns.emplace_back(std::move(conn));
        // TODO respond immediately with most recent data
        conn.wait();
    };
    d_httpServer.addServerEventRoute("/updates", eventHandler);

    int rc = d_httpServer.serve();
    if (0 != rc) {
        fwoop::Log::Error("http server failure, rc=", rc);
    }
}

void Chestnut::pushEvent(const std::string &name, const std::string &value)
{
    fwoop::Log::Info("pushing event: ", name, " value: ", value, " numConns: ", d_activeConns.size());
    auto itr = d_activeConns.begin();
    while (itr != d_activeConns.end()) {
        if (!itr->pushEvent(name, value)) {
            itr = d_activeConns.erase(itr);
        } else {
            itr++;
        }
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
