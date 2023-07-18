#include <fwoop_gaugehistogrammetric.h>
#include <fwoop_gaugemetric.h>
#include <memory>
#include <weather/weatherdatapublisher.h>

namespace chestnut {

WeatherDataPublisher::WeatherDataPublisher(const fwoop::Metric::Labels_t &labels,
                                           const std::shared_ptr<fwoop::GaugeMetricSeries> &currTempSeries,
                                           const std::shared_ptr<fwoop::GaugeMetricSeries> &highTempSeries,
                                           const std::shared_ptr<fwoop::GaugeMetricSeries> &lowTempSeries,
                                           const std::shared_ptr<fwoop::GaugeMetricSeries> &humiditySeries,
                                           const std::shared_ptr<fwoop::GaugeMetricSeries> &windSpeedSeries,
                                           const std::shared_ptr<fwoop::GaugeMetricSeries> &precipChanceSeries,
                                           const std::shared_ptr<fwoop::GaugeHistogramMetricSeries> &tempForecastSeries)
    : d_currTempGauge(currTempSeries->newGauge(labels)), d_highTempGauge(highTempSeries->newGauge(labels)),
      d_lowTempGauge(lowTempSeries->newGauge(labels)), d_humidityGauge(humiditySeries->newGauge(labels)),
      d_windSpeedGauge(windSpeedSeries->newGauge(labels)), d_precipChanceGauge(precipChanceSeries->newGauge(labels))
//   d_tempForecast(tempForecastSeries->newGaugeHistogram(0, 24, 8, labels))
{
}

void WeatherDataPublisher::publish(const WeatherData &data)
{
    d_currTempGauge->set(data.getTemperature());
    d_highTempGauge->set(data.getMaxTemp());
    d_lowTempGauge->set(data.getMinTemp());
    d_humidityGauge->set(data.getHumidity());
    d_windSpeedGauge->set(data.getWindSpeed());
    d_precipChanceGauge->set(data.getPrecipChance() * 100);

    // auto temps = data.getTemps();
    // for (unsigned int i = 0; i < temps.size(); i++) {
    //     d_tempForecast->record(i * 3, temps[i]);
    // }
}

WeatherDataPublisherFactory::WeatherDataPublisherFactory(const std::shared_ptr<Publisher> &publisher)
    : d_publisher(publisher),
      d_currTempSeries(publisher->newGaugeSeries("chestnut_weather_temperature", "fahrenheit", "current temperature")),
      d_highTempSeries(
          d_publisher->newGaugeSeries("chestnut_weather_daily_high_temp", "fahrenheit", "high temperature")),
      d_lowTempSeries(d_publisher->newGaugeSeries("chestnut_weather_daily_low_temp", "fahrenheit", "low temperature")),
      d_humiditySeries(d_publisher->newGaugeSeries("chestnut_weather_daily_humidity", "percent", "humidity")),
      d_windSpeedSeries(
          d_publisher->newGaugeSeries("chestnut_weather_daily_wind_speed", "miles_per_hour", "wind speed")),
      d_precipChanceSeries(
          d_publisher->newGaugeSeries("chestnut_weather_precipitation_chance", "percent", "chance of precipitation"))
//   d_tempForecastSeries(
//       d_publisher->newGaugeHistogramSeries("chestnut_weather_forecast", "fahrenheit", "temperature forecast"))
{
}

std::shared_ptr<WeatherDataPublisher>
WeatherDataPublisherFactory::newWeatherDataPublisher(const std::unordered_map<std::string, std::string> &labels)
{
    return std::make_shared<WeatherDataPublisher>(labels, d_currTempSeries, d_highTempSeries, d_lowTempSeries,
                                                  d_humiditySeries, d_windSpeedSeries, d_precipChanceSeries,
                                                  d_tempForecastSeries);
}

} // namespace chestnut
