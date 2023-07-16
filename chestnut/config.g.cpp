#include <cstddef>
#include <gtest/gtest.h>

#include <config.h>

#include <cstdlib>

TEST(CHESTNUT_CONFIG, Access)
{
    // GIVEN
    const char *prevKey = std::getenv("WEATHER_API_KEY");
    const char *prevUrl = std::getenv("WEATHER_BASE_URL");

    const std::string key = "abcdef";
    const std::string url = "http://weather";

    // WHEN
    ASSERT_EQ(0, setenv("WEATHER_API_KEY", key.c_str(), 1));
    ASSERT_EQ(0, setenv("WEATHER_BASE_URL", url.c_str(), 1));

    // THEN
    EXPECT_EQ(key, chestnut::Config::weatherAPIKey());
    EXPECT_EQ(url, chestnut::Config::weatherBaseURL());

    // FINALLY
    if (prevKey != nullptr) {
        EXPECT_EQ(0, setenv("WEATHER_API_KEY", prevKey, 1));
    }
    if (prevUrl != nullptr) {
        EXPECT_EQ(0, setenv("WEATHER_BASE_URL", prevUrl, 1));
    }
}
