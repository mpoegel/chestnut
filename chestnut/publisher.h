#pragma once

#include <memory>
#include <string>
#include <utility>

#include <fwoop_gaugehistogrammetric.h>
#include <fwoop_gaugemetric.h>
#include <fwoop_openmetrics.h>

class Publisher {
  public:
    virtual int start() = 0;
    virtual void stop() = 0;

    virtual std::shared_ptr<fwoop::CounterMetricSeries>
    newCounterSeries(const std::string &name, const std::string &unit, const std::string &summary = "") = 0;
    virtual std::shared_ptr<fwoop::GaugeMetricSeries> newGaugeSeries(const std::string &name, const std::string &unit,
                                                                     const std::string &summary = "") = 0;
    virtual std::shared_ptr<fwoop::GaugeHistogramMetricSeries>
    newGaugeHistogramSeries(const std::string &name, const std::string &unit, const std::string &summary = "") = 0;
};

class OpenMetricsPublisher : public Publisher {
  private:
    fwoop::OpenMetricsPublisher d_publisher;

  public:
    OpenMetricsPublisher(int port) : d_publisher(port) {}
    ~OpenMetricsPublisher() = default;
    OpenMetricsPublisher(const OpenMetricsPublisher &rhs) = delete;
    OpenMetricsPublisher &operator=(const OpenMetricsPublisher &rhs) = delete;
    OpenMetricsPublisher(OpenMetricsPublisher &&rhs) = default;

    int start() override { return d_publisher.start(); }
    void stop() override { d_publisher.stop(); }
    std::shared_ptr<fwoop::CounterMetricSeries> newCounterSeries(const std::string &name, const std::string &unit,
                                                                 const std::string &summary) override
    {
        return d_publisher.newCounterSeries(name, unit, summary);
    }
    std::shared_ptr<fwoop::GaugeMetricSeries> newGaugeSeries(const std::string &name, const std::string &unit,
                                                             const std::string &summary) override
    {
        return d_publisher.newGaugeSeries(name, unit, summary);
    }
    std::shared_ptr<fwoop::GaugeHistogramMetricSeries>
    newGaugeHistogramSeries(const std::string &name, const std::string &unit, const std::string &summary = "") override
    {
        return d_publisher.newGaugeHistogramSeries(name, unit, summary);
    }
};
