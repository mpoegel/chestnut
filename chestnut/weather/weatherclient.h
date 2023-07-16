#pragma once

#include <memory>
#include <string>
#include <vector>

#include <publisher.h>
#include <weather/weatherdata.h>
#include <weather/weatherdatapublisher.h>

namespace chestnut {

class WeatherClient {
  public:
    enum Unit {
        Standard = 0,
        Metric = 1,
        Imperial = 2,
    };

  private:
    const std::string d_baseUrl;
    const std::string d_lat;
    const std::string d_lon;
    const std::string d_apiKey;
    Unit d_unit;
    WeatherData d_data;

    std::vector<std::shared_ptr<WeatherDataPublisher>> d_forecast;

    std::string unitToString() const;

  public:
    WeatherClient(const std::string &baseUrl, const std::string &lat, const std::string &lon,
                  const std::string &apiKey);
    ~WeatherClient() = default;
    WeatherClient(const WeatherClient &rhs) = default;
    WeatherClient &operator=(const WeatherClient &rhs) = delete;
    WeatherClient(WeatherClient &&rhs) = default;

    bool refresh();
    void setUnit(Unit unit);
    void setPublisher(const std::shared_ptr<Publisher> &publisher);

    const WeatherData &data() const;
};

inline void WeatherClient::setUnit(Unit unit) { d_unit = unit; }
inline const WeatherData &WeatherClient::data() const { return d_data; }

} // namespace chestnut
