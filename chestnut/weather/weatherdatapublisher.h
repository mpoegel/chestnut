#pragma once

#include <fwoop_gaugehistogrammetric.h>
#include <fwoop_gaugemetric.h>
#include <memory>

#include "weather/weatherdata.h"
#include <publisher.h>
#include <string>
#include <unordered_map>

namespace chestnut {

class WeatherDataPublisher {
  private:
    std::shared_ptr<fwoop::GaugeMetric> d_currTempGauge;
    std::shared_ptr<fwoop::GaugeMetric> d_highTempGauge;
    std::shared_ptr<fwoop::GaugeMetric> d_lowTempGauge;
    std::shared_ptr<fwoop::GaugeMetric> d_humidityGauge;
    std::shared_ptr<fwoop::GaugeMetric> d_windSpeedGauge;
    std::shared_ptr<fwoop::GaugeHistogramMetric> d_tempForecast;

  public:
    WeatherDataPublisher(const fwoop::Metric::Labels_t &labels,
                         const std::shared_ptr<fwoop::GaugeMetricSeries> &currTempSeries,
                         const std::shared_ptr<fwoop::GaugeMetricSeries> &highTempSeries,
                         const std::shared_ptr<fwoop::GaugeMetricSeries> &lowTempSeries,
                         const std::shared_ptr<fwoop::GaugeMetricSeries> &humiditySeries,
                         const std::shared_ptr<fwoop::GaugeMetricSeries> &windSpeedSeries,
                         const std::shared_ptr<fwoop::GaugeHistogramMetricSeries> &tempForecastSeries);
    ~WeatherDataPublisher() = default;
    WeatherDataPublisher(const WeatherDataPublisher &rhs) = default;
    WeatherDataPublisher &operator=(const WeatherDataPublisher &rhs) = delete;
    WeatherDataPublisher(WeatherDataPublisher &&rhs) = default;

    void publish(const WeatherData &data);
};

class WeatherDataPublisherFactory {
  private:
    std::shared_ptr<Publisher> d_publisher;
    std::shared_ptr<fwoop::GaugeMetricSeries> d_currTempSeries;
    std::shared_ptr<fwoop::GaugeMetricSeries> d_highTempSeries;
    std::shared_ptr<fwoop::GaugeMetricSeries> d_lowTempSeries;
    std::shared_ptr<fwoop::GaugeMetricSeries> d_humiditySeries;
    std::shared_ptr<fwoop::GaugeMetricSeries> d_windSpeedSeries;
    std::shared_ptr<fwoop::GaugeHistogramMetricSeries> d_tempForecastSeries;

  public:
    WeatherDataPublisherFactory(const std::shared_ptr<Publisher> &publisher);
    ~WeatherDataPublisherFactory() = default;
    WeatherDataPublisherFactory(const WeatherDataPublisherFactory &rhs) = default;
    WeatherDataPublisherFactory &operator=(const WeatherDataPublisherFactory &rhs) = delete;
    WeatherDataPublisherFactory(WeatherDataPublisherFactory &&rhs) = default;

    std::shared_ptr<WeatherDataPublisher> newWeatherDataPublisher(const fwoop::Metric::Labels_t &labels);
};

} // namespace chestnut
