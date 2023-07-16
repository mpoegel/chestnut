#include <chrono>
#include <cstdint>
#include <fwoop_datetime.h>
#include <sys/types.h>
#include <system_error>
#include <weather/weatherdata.h>

#include <fwoop_json.h>
#include <fwoop_log.h>

namespace chestnut {

std::ostream &operator<<(std::ostream &os, const WeatherData &weather)
{
    os << "[ temp=" << weather.getTemperature() << " maxTemp=" << weather.getMaxTemp()
       << " minTemp=" << weather.getMinTemp() << " feelsLike=" << weather.getFeelsLike()
       << " humidity=" << weather.getHumidity() << " windSpeed=" << weather.getWindSpeed() << " ]";
    return os;
}

std::error_code WeatherData::parse(const std::string &data, WeatherData &weather)
{
    uint32_t parsedLength = 0;
    auto json = fwoop::JsonObject((uint8_t *)data.c_str(), data.length(), parsedLength);

    return WeatherData::parse(json, weather);
}

std::error_code WeatherData::parse(const fwoop::JsonObject &json, WeatherData &weather)
{
    auto dt = json.get<int>("dt");
    if (!dt.has_value()) {
        fwoop::Log::Error("no dt");
        return std::make_error_code(std::errc::invalid_argument);
    }
    weather.d_datetime = fwoop::DateTime(dt.value());

    auto main = json.getObject("main");
    if (!main) {
        fwoop::Log::Error("no main");
        return std::make_error_code(std::errc::invalid_argument);
    }

    auto temp = main->getDouble("temp");
    if (!temp.has_value()) {
        fwoop::Log::Error("no temp");
        return std::make_error_code(std::errc::invalid_argument);
    }
    weather.d_temperature = temp.value();
    weather.d_temps.push_back(temp.value());

    auto minTemp = main->getDouble("temp_min");
    if (!minTemp.has_value()) {
        fwoop::Log::Error("no temp_min");
        return std::make_error_code(std::errc::invalid_argument);
    }
    weather.d_minTemp = minTemp.value();

    auto maxTemp = main->getDouble("temp_max");
    if (!maxTemp.has_value()) {
        fwoop::Log::Error("no temp_max");
        return std::make_error_code(std::errc::invalid_argument);
    }
    weather.d_maxTemp = maxTemp.value();

    auto feelsLike = main->getDouble("feels_like");
    if (!feelsLike.has_value()) {
        fwoop::Log::Error("no feels_like");
        return std::make_error_code(std::errc::invalid_argument);
    }
    weather.d_feelsLike = feelsLike.value();

    auto humidity = main->getDouble("humidity");
    if (!humidity.has_value()) {
        fwoop::Log::Error("no humidity");
        return std::make_error_code(std::errc::invalid_argument);
    }
    weather.d_humidity = humidity.value();

    auto wind = json.getObject("wind");
    if (!wind) {
        fwoop::Log::Error("no wind");
        return std::make_error_code(std::errc::invalid_argument);
    }

    auto windSpeed = wind->getDouble("speed");
    if (!windSpeed.has_value()) {
        fwoop::Log::Error("no wind.speed");
        return std::make_error_code(std::errc::invalid_argument);
    }
    weather.d_windSpeed = windSpeed.value();

    return std::error_code();
}

std::error_code MultiDayWeatherData::parse(const std::string &data, MultiDayWeatherData &weather)
{
    uint32_t parsedLength = 0;
    auto json = fwoop::JsonObject((uint8_t *)data.c_str(), data.length(), parsedLength);

    auto list = json.getArray("list");
    for (unsigned int i = 0; i < list->length(); i++) {
        auto obj = list->getObject(i);
        WeatherData weatherReading;
        auto ec = WeatherData::parse(*obj.get(), weatherReading);
        if (ec) {
            return ec;
        }
        if (weather.d_forecast.size() == 0 ||
            weather.d_forecast[weather.d_forecast.size() - 1].d_datetime.day() != weatherReading.d_datetime.day()) {
            weather.d_forecast.push_back(weatherReading);
        } else {
            auto &last = weather.d_forecast[weather.d_forecast.size() - 1];
            // last.d_temperature = std::max(last.d_temperature, weatherReading.d_temperature);
            last.d_temps.push_back(weatherReading.d_temperature);
            last.d_feelsLike = std::max(last.d_feelsLike, weatherReading.d_feelsLike);
            last.d_maxTemp = std::max(last.d_maxTemp, weatherReading.d_maxTemp);
            last.d_minTemp = std::min(last.d_minTemp, weatherReading.d_minTemp);
            last.d_humidity = std::max(last.d_humidity, weatherReading.d_humidity);
            last.d_windSpeed = std::max(last.d_windSpeed, weatherReading.d_windSpeed);
        }
    }

    return std::error_code();
}

} // namespace chestnut
