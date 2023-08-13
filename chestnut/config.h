#pragma once

#include <ostream>
#include <string>

namespace chestnut {

class Config {
  private:
    static Config *s_config_p;

    int d_metricsPort;
    int d_refreshInterval;

    bool d_isWeatherEnabled;
    std::string d_weatherAPIKey;
    std::string d_weatherBaseURL;
    std::string d_weatherLat;
    std::string d_weatherLon;
    int d_weatherDaysAhead;
    int d_timezone;
    bool d_debug;

    Config() = default;
    Config(const Config &rhs) = default;
    Config &operator=(const Config &rhs) = default;
    Config(Config &&rhs) = default;

    static Config &singleton();
    static int loadInt(const char *name, int defaultValue);
    static bool loadBool(const char *name, bool defaultValue = false);
    static std::string loadString(const char *name, const std::string &defaultValue = "");

  public:
    ~Config() = default;

    static const int metricsPort();
    static const int refreshInterval();
    static const bool isWeatherEnabled();
    static const std::string weatherAPIKey();
    static const std::string weatherBaseURL();
    static const std::string weatherLat();
    static const std::string weatherLon();
    static const int timezone();
    static const bool debug();

    static void reload();
};

std::ostream &operator<<(std::ostream &os, Config *cfg);

inline const int Config::metricsPort() { return singleton().d_metricsPort; }
inline const int Config::refreshInterval() { return singleton().d_refreshInterval; }
inline const bool Config::isWeatherEnabled() { return singleton().d_isWeatherEnabled; }
inline const std::string Config::weatherAPIKey() { return singleton().d_weatherAPIKey; }
inline const std::string Config::weatherBaseURL() { return singleton().d_weatherBaseURL; }
inline const std::string Config::weatherLat() { return singleton().d_weatherLat; }
inline const std::string Config::weatherLon() { return singleton().d_weatherLon; }
inline const int Config::timezone() { return singleton().d_timezone; }
inline const bool Config::debug() { return singleton().d_debug; }

} // namespace chestnut
