#pragma once

#include <fwoop_httpserver.h>
#include <memory.h>

#include <memory>
#include <publisher.h>
#include <thread>
#include <weather/weatherclient.h>

namespace chestnut {

class Chestnut {
  private:
    std::shared_ptr<Publisher> d_publisher;
    std::shared_ptr<WeatherClient> d_weatherClient;
    fwoop::HttpServer d_httpServer;
    std::thread d_refreshThread;
    std::thread d_httpThread;
    bool d_needStop;

    void refresh();
    void serveHttp();

  public:
    Chestnut();
    Chestnut(std::shared_ptr<Publisher> publisher);

    ~Chestnut();
    Chestnut(const Chestnut &rhs) = delete;
    Chestnut &operator=(const Chestnut &rhs) = delete;
    Chestnut(Chestnut &&rhs) = default;

    int start();
    void stop();
};

inline Chestnut::~Chestnut() { stop(); }

} // namespace chestnut
