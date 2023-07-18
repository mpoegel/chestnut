#pragma once

#include <ctime>
#include <ostream>
#include <string>
#include <vector>

#include <fwoop_datetime.h>
#include <fwoop_json.h>

namespace chestnut {

class WeatherData;
class MultiDayWeatherData;

class WeatherData {
  private:
    std::vector<double> d_temps;
    double d_temperature;
    double d_maxTemp;
    double d_minTemp;
    double d_feelsLike;
    int d_humidity;
    double d_windSpeed;
    double d_precipChance;

    fwoop::DateTime d_datetime;

  public:
    static std::error_code parse(const std::string &data, WeatherData &weather);
    static std::error_code parse(const fwoop::JsonObject &json, WeatherData &weather);

    WeatherData();
    WeatherData(const fwoop::Json &json);

    ~WeatherData() = default;
    WeatherData(const WeatherData &rhs) = default;
    WeatherData &operator=(const WeatherData &rhs) = default;
    WeatherData(WeatherData &&rhs) = default;

    const std::vector<double> getTemps() const;
    double getTemperature() const;
    double getMaxTemp() const;
    double getMinTemp() const;
    double getFeelsLike() const;
    int getHumidity() const;
    double getWindSpeed() const;
    double getPrecipChance() const;

    fwoop::DateTime getDateTime() const;

    friend MultiDayWeatherData;
};

std::ostream &operator<<(std::ostream &os, const WeatherData &weather);

inline WeatherData::WeatherData() : d_datetime(0) {}
inline const std::vector<double> WeatherData::getTemps() const { return d_temps; }
inline double WeatherData::getTemperature() const { return d_temperature; }
inline double WeatherData::getMaxTemp() const { return d_maxTemp; }
inline double WeatherData::getMinTemp() const { return d_minTemp; }
inline double WeatherData::getFeelsLike() const { return d_feelsLike; }
inline int WeatherData::getHumidity() const { return d_humidity; }
inline double WeatherData::getWindSpeed() const { return d_windSpeed; }
inline double WeatherData::getPrecipChance() const { return d_precipChance; }
inline fwoop::DateTime WeatherData::getDateTime() const { return d_datetime; }

class MultiDayWeatherData {
  private:
    std::vector<WeatherData> d_forecast;

  public:
    static std::error_code parse(const std::string &data, MultiDayWeatherData &weather);

    MultiDayWeatherData() = default;
    ~MultiDayWeatherData() = default;
    MultiDayWeatherData(const MultiDayWeatherData &rhs) = default;
    MultiDayWeatherData &operator=(const MultiDayWeatherData &rhs) = default;
    MultiDayWeatherData(MultiDayWeatherData &&rhs) = default;

    unsigned int getNumDays() const;
    const WeatherData getDaily(unsigned int day) const;
};

inline unsigned int MultiDayWeatherData::getNumDays() const { return d_forecast.size(); }
inline const WeatherData MultiDayWeatherData::getDaily(unsigned int day) const
{
    return day < d_forecast.size() ? d_forecast[day] : WeatherData();
}

} // namespace chestnut
