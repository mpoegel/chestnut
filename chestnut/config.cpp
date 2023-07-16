#include <algorithm>
#include <config.h>

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>

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

    s_config_p->d_debug = loadBool("DEBUG");

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

} // namespace chestnut
