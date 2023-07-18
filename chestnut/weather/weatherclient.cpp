#include <config.h>
#include <weather/weatherclient.h>
#include <weather/weatherdatapublisher.h>

#include <fwoop_gaugemetric.h>
#include <fwoop_httpclient.h>
#include <fwoop_httpheader.h>
#include <fwoop_httprequest.h>
#include <fwoop_httpresponse.h>
#include <fwoop_log.h>
#include <fwoop_metric.h>

#include <cstdint>
#include <memory>
#include <utility>

namespace chestnut {

namespace {
constexpr unsigned int DAYS_AHEAD = 4;
const std::string DAY_OF_WEEK[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
} // namespace

WeatherClient::WeatherClient(const std::string &baseUrl, const std::string &lat, const std::string &lon,
                             const std::string &apiKey)
    : d_baseUrl(baseUrl), d_lat(lat), d_lon(lon), d_apiKey(apiKey), d_unit(Unit::Imperial)
{
}

std::string WeatherClient::unitToString() const
{
    switch (d_unit) {
    case Standard:
        return "standard";
    case Metric:
        return "metric";
    case Imperial:
    default:
        return "imperial";
    }
}

bool WeatherClient::refresh()
{
    fwoop::Log::Debug("refreshing weather, baseUrl=", d_baseUrl);
    fwoop::HttpClient client(d_baseUrl, 80);

    fwoop::HttpRequest request;
    request.addHeader(fwoop::HttpHeader::Host, d_baseUrl);
    request.addHeader(fwoop::HttpHeader::Accept, "*/*");
    request.setPath("/data/2.5/forecast?lat=" + d_lat + "&lon=" + d_lon + "&units=" + unitToString() +
                    "&appid=" + d_apiKey);
    request.setMethod(fwoop::HttpRequest::Method::Get);
    request.setVersion(fwoop::HttpVersion::Http1_1);

    fwoop::Log::Debug("sending weather request, ", request);

    auto response = std::make_shared<fwoop::HttpResponse>();

    auto ec = client.makeReqest(request, response);
    if (ec) {
        fwoop::Log::Error("request failed: ", ec.message());
        return false;
    }

    fwoop::Log::Debug("response: ", *response.get());
    fwoop::Log::Debug("response body: ", response->getBody());

    MultiDayWeatherData weather;
    ec = MultiDayWeatherData::parse(response->getBody(), weather);
    if (ec) {
        fwoop::Log::Error("failed to parse weather forecast: ", ec, " body=", response->getBody());
        return false;
    }

    for (unsigned int i = 0; i < weather.getNumDays() && i <= DAYS_AHEAD; i++) {
        d_forecast[i]->publish(weather.getDaily(i));
    }
    return true;
}

void WeatherClient::setPublisher(const std::shared_ptr<Publisher> &publisher)
{
    auto factory = WeatherDataPublisherFactory(publisher);
    auto now = fwoop::DateTime::now();
    now.addHours(Config::timezone());
    for (unsigned int i = 0; i <= DAYS_AHEAD; i++) {
        uint8_t dow = (now.dayOfWeek() + i) % 7;
        fwoop::Metric::Labels_t labels = {{"days_in_future", std::to_string(i)}, {"day_of_week", DAY_OF_WEEK[dow]}};
        d_forecast.push_back(factory.newWeatherDataPublisher(labels));
    }
}

} // namespace chestnut
