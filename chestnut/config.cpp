#include <config.h>

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fwoop_log.h>
#include <memory>
#include <string>

#include <fwoop_dnsquery.h>

namespace chestnut {

Config *Config::s_config_p = nullptr;

void Config::reload()
{
    delete s_config_p;
    s_config_p = nullptr;
    singleton();
}

Config &Config::singleton()
{
    if (s_config_p != nullptr) {
        return *s_config_p;
    }

    s_config_p = new Config();
    s_config_p->d_metricsPort = loadInt("METRICS_PORT", 3003);
    s_config_p->d_refreshInterval = loadInt("REFRESH_INTERVAL", 300);
    s_config_p->d_isWeatherEnabled = loadBool("ENABLE_WEATHER");
    s_config_p->d_weatherAPIKey = loadString("WEATHER_API_KEY");
    s_config_p->d_weatherBaseURL = loadString("WEATHER_BASE_URL");
    s_config_p->d_weatherLat = loadString("WEATHER_LAT");
    s_config_p->d_weatherLon = loadString("WEATHER_LON");
    s_config_p->d_timezone = loadInt("WEATHER_TIMEZONE", 0);

    s_config_p->d_debug = loadBool("DEBUG");
    ;

    auto dnsServerAddr = loadString("DNS_SERVER_ADDR", "127.0.0.1");
    fwoop::DNS::Query::ServerAddress = dnsServerAddr;

    fwoop::Log::Debug("config loaded: ", s_config_p);
    fwoop::Log::Debug("DNS_SERVER_ADDR: ", dnsServerAddr);

    return *s_config_p;
}

int Config::loadInt(const char *name, int defaultValue)
{
    const char *raw = std::getenv(name);
    if (raw == nullptr) {
        return defaultValue;
    }
    return std::atoi(raw);
}

bool Config::loadBool(const char *name, bool defaultValue)
{
    const char *raw = std::getenv(name);
    if (raw == nullptr) {
        return defaultValue;
    }
    if (0 == strcmp("1", raw)) {
        return true;
    }
    if (0 == strcmp("true", raw)) {
        return true;
    }
    return true;
}

std::string Config::loadString(const char *name, const std::string &defaultValue)
{
    const char *raw = std::getenv(name);
    if (raw == nullptr) {
        return defaultValue;
    }
    return std::string(raw);
}

std::ostream &operator<<(std::ostream &os, Config *cfg)
{
    os << "[ metricsPort: " << cfg->metricsPort() << ", "
       << "refreshInterval: " << cfg->refreshInterval() << ", "
       << "isWeatherEnabled: " << cfg->isWeatherEnabled() << ", "
       << "weatherAPIKey: ******, "
       << "weatherBaseURL: " << cfg->weatherBaseURL() << ", "
       << "weatherLat: " << cfg->weatherLat() << ", "
       << "weatherLon: " << cfg->weatherLon() << ", "
       << "timezone: " << cfg->timezone() << ", "
       << "debug: " << cfg->debug();
    os << " ]";
    return os;
}

} // namespace chestnut
