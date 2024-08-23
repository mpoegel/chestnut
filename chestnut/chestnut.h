#pragma once

#include <condition_variable>
#include <fwoop_httprequest.h>
#include <fwoop_httpserver.h>
#include <fwoop_httpserverevent.h>
#include <fwoop_log.h>
#include <list>
#include <memory.h>
#include <memory>
#include <mutex>
#include <thread>

#include <event_callback.h>
#include <publisher.h>
#include <weather/weatherclient.h>

namespace chestnut {

class Connection;

class Chestnut : public EventCallback {
  private:
    std::shared_ptr<Publisher> d_publisher;
    std::shared_ptr<WeatherClient> d_weatherClient;
    fwoop::HttpServer d_httpServer;
    std::thread d_refreshThread;
    std::thread d_httpThread;
    bool d_needStop;
    std::list<Connection> d_activeConns;

    void refresh();
    void serveHttp();

  public:
    Chestnut();
    Chestnut(std::shared_ptr<Publisher> publisher);

    ~Chestnut();
    Chestnut(const Chestnut &rhs) = delete;
    Chestnut &operator=(const Chestnut &rhs) = delete;
    Chestnut(Chestnut &&rhs) = delete;

    int start();
    void stop();

    // from EventCallback
    void pushEvent(const std::string &name, const std::string &value) override;
};

class Connection {
  private:
    const fwoop::HttpRequest &d_request;
    fwoop::HttpServerEvent &d_serverEvent;
    std::condition_variable d_exitCond;
    std::mutex d_exitLock;

  public:
    Connection(const fwoop::HttpRequest &request, fwoop::HttpServerEvent &serverEvent)
        : d_request(request), d_serverEvent(serverEvent)
    {
    }
    Connection(Connection &&rhs) : d_request(rhs.d_request), d_serverEvent(rhs.d_serverEvent) {}
    ~Connection() {}

    bool pushEvent(const std::string &event, const std::string &data)
    {
        fwoop::Log::Debug("forwarding event: ", event);
        if (!d_serverEvent.pushEvent(event, data)) {
            fwoop::Log::Warn("push event failed");
            d_exitCond.notify_all();
            return false;
        }
        return true;
    }
    void wait()
    {
        std::unique_lock lock(d_exitLock);
        d_exitCond.wait(lock);
    }
    void close() { d_exitCond.notify_all(); }
};

inline Chestnut::~Chestnut() { stop(); }

} // namespace chestnut
