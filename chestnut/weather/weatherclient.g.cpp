#include <gtest/gtest.h>

#include <config.h>
#include <weather/weatherclient.h>

TEST(CHESTNUT_WEATHERCLIENT, DISABLED_Refresh)
{
    // GIVEN
    chestnut::Config::reload();
    chestnut::WeatherClient client(chestnut::Config::weatherBaseURL(), chestnut::Config::weatherLat(),
                                   chestnut::Config::weatherLon(), chestnut::Config::weatherAPIKey());

    // WHEN
    ASSERT_TRUE(client.refresh());

    // THEN
}
