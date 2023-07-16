#include <chestnut.h>

#include <chrono>
#include <config.h>
#include <functional>
#include <memory>
#include <publisher.h>
#include <thread>

#include <fwoop_log.h>

namespace chestnut {

Chestnut::Chestnut()
    : d_publisher(std::make_shared<OpenMetricsPublisher>(Config::metricsPort())),
      d_weatherClient(std::make_shared<WeatherClient>(Config::weatherBaseURL(), Config::weatherLat(),
                                                      Config::weatherLon(), Config::weatherAPIKey())),
      d_needStop(false)
{
}

Chestnut::Chestnut(std::shared_ptr<Publisher> publisher) : d_publisher(publisher), d_needStop(false) {}

void Chestnut::refresh()
{
    while (!d_needStop) {
        if (Config::isWeatherEnabled()) {
            d_weatherClient->refresh();
        }
        std::this_thread::sleep_for(std::chrono::seconds(Config::refreshInterval()));
    }
}

int Chestnut::start()
{
    if (!Config::debug()) {
        fwoop::Log::SetThreshold(fwoop::Log::e_Info);
    }

    d_publisher->start();
    d_weatherClient->setPublisher(d_publisher);
    refresh();
    return 0;
}

void Chestnut::stop()
{
    d_needStop = true;
    d_publisher->stop();
    d_refreshThread.join();
}

} // namespace chestnut
