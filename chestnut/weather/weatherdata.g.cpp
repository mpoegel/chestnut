#include <gtest/gtest.h>

#include <config.h>
#include <weather/weatherdata.h>

TEST(CHESTNUT_WEATHERDATA, Parse)
{
    // GIVEN
    const std::string data =
        "{"
        "\"coord\" : {\"lon\" : -73.9497, \"lat\" : 40.6526},"
        "\"weather\" : [ {\"id\" : 800, \"main\" : \"Clear\", \"description\" : \"clear sky\", \"icon\" : \"01d\"} ],"
        "\"base\" : \"stations\","
        "\"main\" : {"
        "    \"temp\" : 45.25,"
        "    \"feels_like\" : 39.24,"
        "    \"temp_min\" : 41.32,"
        "    \"temp_max\" : 48.11,"
        "    \"pressure\" : 1033,"
        "    \"humidity\" : 48"
        "},"
        "\"visibility\" : 10000,"
        "\"wind\" : {\"speed\" : 12.66, \"deg\" : 40},"
        "\"clouds\" : {\"all\" : 0},"
        "\"dt\" : 1681044900,"
        "\"sys\" : {\"type\" : 2, \"id\" : 2080536, \"country\" : \"US\", \"sunrise\" : 1681036000, \"sunset\" : "
        "1681082866},"
        "\"timezone\" : -14400,"
        "\"id\" : 5110302,"
        "\"name\" : \"Brooklyn\","
        "\"cod\" : 200"
        "}";
    chestnut::WeatherData weather;

    // WHEN
    auto ec = chestnut::WeatherData::parse(data, weather);

    // THEN
    EXPECT_FALSE(ec);
    EXPECT_DOUBLE_EQ(45.25, weather.getTemperature());
    EXPECT_DOUBLE_EQ(41.32, weather.getMinTemp());
    EXPECT_DOUBLE_EQ(48.11, weather.getMaxTemp());
    EXPECT_DOUBLE_EQ(39.24, weather.getFeelsLike());
    EXPECT_EQ(48, weather.getHumidity());
    EXPECT_DOUBLE_EQ(12.66, weather.getWindSpeed());
}

TEST(CHESTNUT_WEATHERDATA, Parse2)
{
    // GIVEN
    const std::string data =
        "{"
        "\"coord\": {\"lon\":-73.9496,\"lat\":40.6501},"
        "\"weather\": [{\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast clouds\",\"icon\":\"04d\"}],"
        "\"base\": \"stations\","
        "\"main\": "
        "{\"temp\":65.25,\"feels_like\":64.85,\"temp_min\":62.85,\"temp_max\":68.88,\"pressure\":1016,\"humidity\":72},"
        "\"visibility\":10000,"
        "\"wind\":{\"speed\":17.27,\"deg\":60},"
        "\"clouds\":{\"all\":100},"
        "\"dt\":1685811607,"
        "\"sys\":{\"type\":2,\"id\":2080536,\"country\":\"US\",\"sunrise\":1685784392,\"sunset\":1685838098},"
        "\"timezone\":-14400,"
        "\"id\":5110302,"
        "\"name\":\"Brooklyn\","
        "\"cod\":200"
        "}";
    chestnut::WeatherData weather;

    // WHEN
    auto ec = chestnut::WeatherData::parse(data, weather);

    // THEN
    EXPECT_FALSE(ec);
    EXPECT_DOUBLE_EQ(65.25, weather.getTemperature());
    EXPECT_DOUBLE_EQ(64.85, weather.getFeelsLike());
    EXPECT_EQ(72, weather.getHumidity());
    EXPECT_DOUBLE_EQ(17.27, weather.getWindSpeed());
}

TEST(CHESTNUT_MULTIDAYWEATHERDATA, Parse)
{
    // GIVEN
    const std::string data =
        "{\"cod\":\"200\",\"message\":0,\"cnt\":40,\"list\":[{\"dt\":1687219200,\"main\":{\"temp\":70.86,\"feels_"
        "like\":70.99,\"temp_min\":70.83,\"temp_max\":70.86,\"pressure\":1018,\"sea_level\":1018,\"grnd_level\":1016,"
        "\"humidity\":71,\"temp_kf\":0.02},\"weather\":[{\"id\":801,\"main\":\"Clouds\",\"description\":\"few "
        "clouds\",\"icon\":\"02d\"}],\"clouds\":{\"all\":20},\"wind\":{\"speed\":9.78,\"deg\":138,\"gust\":13.2},"
        "\"visibility\":10000,\"pop\":0.03,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-20 "
        "00:00:00\"},{\"dt\":1687230000,\"main\":{\"temp\":70.43,\"feels_like\":70.47,\"temp_min\":69.55,\"temp_max\":"
        "70.43,\"pressure\":1019,\"sea_level\":1019,\"grnd_level\":1018,\"humidity\":70,\"temp_kf\":0.49},\"weather\":["
        "{\"id\":802,\"main\":\"Clouds\",\"description\":\"scattered "
        "clouds\",\"icon\":\"03n\"}],\"clouds\":{\"all\":41},\"wind\":{\"speed\":8.86,\"deg\":119,\"gust\":12.28},"
        "\"visibility\":10000,\"pop\":0.05,\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-20 "
        "03:00:00\"},{\"dt\":1687240800,\"main\":{\"temp\":68.54,\"feels_like\":68.72,\"temp_min\":67.37,\"temp_max\":"
        "68.54,\"pressure\":1019,\"sea_level\":1019,\"grnd_level\":1018,\"humidity\":77,\"temp_kf\":0.65},\"weather\":["
        "{\"id\":803,\"main\":\"Clouds\",\"description\":\"broken "
        "clouds\",\"icon\":\"04n\"}],\"clouds\":{\"all\":68},\"wind\":{\"speed\":8.75,\"deg\":83,\"gust\":11.03},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-20 "
        "06:00:00\"},{\"dt\":1687251600,\"main\":{\"temp\":65.79,\"feels_like\":66.02,\"temp_min\":65.79,\"temp_max\":"
        "65.79,\"pressure\":1020,\"sea_level\":1020,\"grnd_level\":1018,\"humidity\":84,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04n\"}],\"clouds\":{\"all\":85},\"wind\":{\"speed\":9.98,\"deg\":72,\"gust\":12.64},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-20 "
        "09:00:00\"},{\"dt\":1687262400,\"main\":{\"temp\":66.67,\"feels_like\":66.85,\"temp_min\":66.67,\"temp_max\":"
        "66.67,\"pressure\":1022,\"sea_level\":1022,\"grnd_level\":1020,\"humidity\":81,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":92},\"wind\":{\"speed\":9.91,\"deg\":65,\"gust\":11.21},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-20 "
        "12:00:00\"},{\"dt\":1687273200,\"main\":{\"temp\":71.91,\"feels_like\":72,\"temp_min\":71.91,\"temp_max\":71."
        "91,\"pressure\":1022,\"sea_level\":1022,\"grnd_level\":1021,\"humidity\":68,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":10.94,\"deg\":83,\"gust\":11.18},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-20 "
        "15:00:00\"},{\"dt\":1687284000,\"main\":{\"temp\":72.99,\"feels_like\":73.09,\"temp_min\":72.99,\"temp_max\":"
        "72.99,\"pressure\":1022,\"sea_level\":1022,\"grnd_level\":1020,\"humidity\":66,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":12.64,\"deg\":110,\"gust\":12.59},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-20 "
        "18:00:00\"},{\"dt\":1687294800,\"main\":{\"temp\":71.47,\"feels_like\":71.51,\"temp_min\":71.47,\"temp_max\":"
        "71.47,\"pressure\":1022,\"sea_level\":1022,\"grnd_level\":1020,\"humidity\":68,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":13.76,\"deg\":127,\"gust\":14.09},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-20 "
        "21:00:00\"},{\"dt\":1687305600,\"main\":{\"temp\":67.8,\"feels_like\":67.71,\"temp_min\":67.8,\"temp_max\":67."
        "8,\"pressure\":1023,\"sea_level\":1023,\"grnd_level\":1021,\"humidity\":73,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":13.18,\"deg\":112,\"gust\":15.5},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-21 "
        "00:00:00\"},{\"dt\":1687316400,\"main\":{\"temp\":65.86,\"feels_like\":65.62,\"temp_min\":65.86,\"temp_max\":"
        "65.86,\"pressure\":1024,\"sea_level\":1024,\"grnd_level\":1022,\"humidity\":74,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04n\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":12.35,\"deg\":91,\"gust\":15.59},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-21 "
        "03:00:00\"},{\"dt\":1687327200,\"main\":{\"temp\":64.69,\"feels_like\":64.24,\"temp_min\":64.69,\"temp_max\":"
        "64.69,\"pressure\":1024,\"sea_level\":1024,\"grnd_level\":1022,\"humidity\":72,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04n\"}],\"clouds\":{\"all\":96},\"wind\":{\"speed\":11.43,\"deg\":72,\"gust\":16.02},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-21 "
        "06:00:00\"},{\"dt\":1687338000,\"main\":{\"temp\":63.16,\"feels_like\":62.65,\"temp_min\":63.16,\"temp_max\":"
        "63.16,\"pressure\":1024,\"sea_level\":1024,\"grnd_level\":1022,\"humidity\":74,\"temp_kf\":0},\"weather\":[{"
        "\"id\":803,\"main\":\"Clouds\",\"description\":\"broken "
        "clouds\",\"icon\":\"04n\"}],\"clouds\":{\"all\":67},\"wind\":{\"speed\":12.71,\"deg\":72,\"gust\":18.54},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-21 "
        "09:00:00\"},{\"dt\":1687348800,\"main\":{\"temp\":64.49,\"feels_like\":63.97,\"temp_min\":64.49,\"temp_max\":"
        "64.49,\"pressure\":1025,\"sea_level\":1025,\"grnd_level\":1023,\"humidity\":71,\"temp_kf\":0},\"weather\":[{"
        "\"id\":803,\"main\":\"Clouds\",\"description\":\"broken "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":82},\"wind\":{\"speed\":15.57,\"deg\":71,\"gust\":20.45},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-21 "
        "12:00:00\"},{\"dt\":1687359600,\"main\":{\"temp\":69.06,\"feels_like\":68.45,\"temp_min\":69.06,\"temp_max\":"
        "69.06,\"pressure\":1025,\"sea_level\":1025,\"grnd_level\":1024,\"humidity\":59,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":16.78,\"deg\":84,\"gust\":19.82},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-21 "
        "15:00:00\"},{\"dt\":1687370400,\"main\":{\"temp\":71.38,\"feels_like\":70.75,\"temp_min\":71.38,\"temp_max\":"
        "71.38,\"pressure\":1025,\"sea_level\":1025,\"grnd_level\":1023,\"humidity\":54,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":18.07,\"deg\":104,\"gust\":20.45},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-21 "
        "18:00:00\"},{\"dt\":1687381200,\"main\":{\"temp\":69.46,\"feels_like\":68.77,\"temp_min\":69.46,\"temp_max\":"
        "69.46,\"pressure\":1024,\"sea_level\":1024,\"grnd_level\":1022,\"humidity\":57,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":17.83,\"deg\":98,\"gust\":20.87},"
        "\"visibility\":10000,\"pop\":0.03,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-21 "
        "21:00:00\"},{\"dt\":1687392000,\"main\":{\"temp\":66.49,\"feels_like\":65.89,\"temp_min\":66.49,\"temp_max\":"
        "66.49,\"pressure\":1025,\"sea_level\":1025,\"grnd_level\":1023,\"humidity\":65,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":16.46,\"deg\":95,\"gust\":20.94},"
        "\"visibility\":10000,\"pop\":0.08,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-22 "
        "00:00:00\"},{\"dt\":1687402800,\"main\":{\"temp\":64.83,\"feels_like\":64.63,\"temp_min\":64.83,\"temp_max\":"
        "64.83,\"pressure\":1025,\"sea_level\":1025,\"grnd_level\":1023,\"humidity\":77,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04n\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":15.23,\"deg\":94,\"gust\":20.4},"
        "\"visibility\":10000,\"pop\":0.1,\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-22 "
        "03:00:00\"},{\"dt\":1687413600,\"main\":{\"temp\":64.51,\"feels_like\":64.36,\"temp_min\":64.51,\"temp_max\":"
        "64.51,\"pressure\":1023,\"sea_level\":1023,\"grnd_level\":1022,\"humidity\":79,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04n\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":14.05,\"deg\":84,\"gust\":19.08},"
        "\"visibility\":10000,\"pop\":0.07,\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-22 "
        "06:00:00\"},{\"dt\":1687424400,\"main\":{\"temp\":64.02,\"feels_like\":63.88,\"temp_min\":64.02,\"temp_max\":"
        "64.02,\"pressure\":1023,\"sea_level\":1023,\"grnd_level\":1021,\"humidity\":80,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04n\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":15.79,\"deg\":72,\"gust\":22.24},"
        "\"visibility\":10000,\"pop\":0.35,\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-22 "
        "09:00:00\"},{\"dt\":1687435200,\"main\":{\"temp\":64.13,\"feels_like\":63.91,\"temp_min\":64.13,\"temp_max\":"
        "64.13,\"pressure\":1024,\"sea_level\":1024,\"grnd_level\":1022,\"humidity\":78,\"temp_kf\":0},\"weather\":[{"
        "\"id\":500,\"main\":\"Rain\",\"description\":\"light "
        "rain\",\"icon\":\"10d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":17.29,\"deg\":71,\"gust\":25.72},"
        "\"visibility\":10000,\"pop\":0.43,\"rain\":{\"3h\":0.34},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-22 "
        "12:00:00\"},{\"dt\":1687446000,\"main\":{\"temp\":64.81,\"feels_like\":64.62,\"temp_min\":64.81,\"temp_max\":"
        "64.81,\"pressure\":1024,\"sea_level\":1024,\"grnd_level\":1022,\"humidity\":77,\"temp_kf\":0},\"weather\":[{"
        "\"id\":500,\"main\":\"Rain\",\"description\":\"light "
        "rain\",\"icon\":\"10d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":17.4,\"deg\":75,\"gust\":26.84},"
        "\"visibility\":10000,\"pop\":0.59,\"rain\":{\"3h\":0.84},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-22 "
        "15:00:00\"},{\"dt\":1687456800,\"main\":{\"temp\":65.34,\"feels_like\":65.23,\"temp_min\":65.34,\"temp_max\":"
        "65.34,\"pressure\":1023,\"sea_level\":1023,\"grnd_level\":1021,\"humidity\":78,\"temp_kf\":0},\"weather\":[{"
        "\"id\":500,\"main\":\"Rain\",\"description\":\"light "
        "rain\",\"icon\":\"10d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":16.96,\"deg\":79,\"gust\":26.19},"
        "\"visibility\":10000,\"pop\":0.64,\"rain\":{\"3h\":0.52},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-22 "
        "18:00:00\"},{\"dt\":1687467600,\"main\":{\"temp\":65.61,\"feels_like\":65.71,\"temp_min\":65.61,\"temp_max\":"
        "65.61,\"pressure\":1021,\"sea_level\":1021,\"grnd_level\":1020,\"humidity\":82,\"temp_kf\":0},\"weather\":[{"
        "\"id\":500,\"main\":\"Rain\",\"description\":\"light "
        "rain\",\"icon\":\"10d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":15.82,\"deg\":72,\"gust\":25.48},"
        "\"visibility\":10000,\"pop\":0.45,\"rain\":{\"3h\":0.27},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-22 "
        "21:00:00\"},{\"dt\":1687478400,\"main\":{\"temp\":64.76,\"feels_like\":65.21,\"temp_min\":64.76,\"temp_max\":"
        "64.76,\"pressure\":1021,\"sea_level\":1021,\"grnd_level\":1019,\"humidity\":91,\"temp_kf\":0},\"weather\":[{"
        "\"id\":500,\"main\":\"Rain\",\"description\":\"light "
        "rain\",\"icon\":\"10d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":14.54,\"deg\":76,\"gust\":21.14},"
        "\"visibility\":10000,\"pop\":0.4,\"rain\":{\"3h\":0.15},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-23 "
        "00:00:00\"},{\"dt\":1687489200,\"main\":{\"temp\":65.1,\"feels_like\":65.73,\"temp_min\":65.1,\"temp_max\":65."
        "1,\"pressure\":1021,\"sea_level\":1021,\"grnd_level\":1019,\"humidity\":94,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04n\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":12.17,\"deg\":67,\"gust\":17.81},"
        "\"visibility\":10000,\"pop\":0.01,\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-23 "
        "03:00:00\"},{\"dt\":1687500000,\"main\":{\"temp\":65.98,\"feels_like\":66.79,\"temp_min\":65.98,\"temp_max\":"
        "65.98,\"pressure\":1020,\"sea_level\":1020,\"grnd_level\":1018,\"humidity\":96,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04n\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":10.04,\"deg\":68,\"gust\":16.13},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-23 "
        "06:00:00\"},{\"dt\":1687510800,\"main\":{\"temp\":66.67,\"feels_like\":67.64,\"temp_min\":66.67,\"temp_max\":"
        "66.67,\"pressure\":1019,\"sea_level\":1019,\"grnd_level\":1017,\"humidity\":98,\"temp_kf\":0},\"weather\":[{"
        "\"id\":500,\"main\":\"Rain\",\"description\":\"light "
        "rain\",\"icon\":\"10n\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":8.88,\"deg\":84,\"gust\":14.25},"
        "\"visibility\":3817,\"pop\":0.2,\"rain\":{\"3h\":0.11},\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-23 "
        "09:00:00\"},{\"dt\":1687521600,\"main\":{\"temp\":68.97,\"feels_like\":69.94,\"temp_min\":68.97,\"temp_max\":"
        "68.97,\"pressure\":1020,\"sea_level\":1020,\"grnd_level\":1018,\"humidity\":93,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":5.32,\"deg\":111,\"gust\":7.02},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-23 "
        "12:00:00\"},{\"dt\":1687532400,\"main\":{\"temp\":74.84,\"feels_like\":75.6,\"temp_min\":74.84,\"temp_max\":"
        "74.84,\"pressure\":1020,\"sea_level\":1020,\"grnd_level\":1018,\"humidity\":76,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":5.73,\"deg\":150,\"gust\":8.39},"
        "\"visibility\":10000,\"pop\":0.01,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-23 "
        "15:00:00\"},{\"dt\":1687543200,\"main\":{\"temp\":72.84,\"feels_like\":73.63,\"temp_min\":72.84,\"temp_max\":"
        "72.84,\"pressure\":1019,\"sea_level\":1019,\"grnd_level\":1017,\"humidity\":81,\"temp_kf\":0},\"weather\":[{"
        "\"id\":500,\"main\":\"Rain\",\"description\":\"light "
        "rain\",\"icon\":\"10d\"}],\"clouds\":{\"all\":97},\"wind\":{\"speed\":8.08,\"deg\":148,\"gust\":12.17},"
        "\"visibility\":10000,\"pop\":0.28,\"rain\":{\"3h\":0.25},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-23 "
        "18:00:00\"},{\"dt\":1687554000,\"main\":{\"temp\":75.54,\"feels_like\":76.32,\"temp_min\":75.54,\"temp_max\":"
        "75.54,\"pressure\":1017,\"sea_level\":1017,\"grnd_level\":1016,\"humidity\":75,\"temp_kf\":0},\"weather\":[{"
        "\"id\":500,\"main\":\"Rain\",\"description\":\"light "
        "rain\",\"icon\":\"10d\"}],\"clouds\":{\"all\":96},\"wind\":{\"speed\":12.64,\"deg\":150,\"gust\":18.99},"
        "\"visibility\":10000,\"pop\":0.42,\"rain\":{\"3h\":0.13},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-23 "
        "21:00:00\"},{\"dt\":1687564800,\"main\":{\"temp\":73.33,\"feels_like\":74.07,\"temp_min\":73.33,\"temp_max\":"
        "73.33,\"pressure\":1017,\"sea_level\":1017,\"grnd_level\":1015,\"humidity\":79,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":98},\"wind\":{\"speed\":12.41,\"deg\":164,\"gust\":22.28},"
        "\"visibility\":10000,\"pop\":0.33,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-24 "
        "00:00:00\"},{\"dt\":1687575600,\"main\":{\"temp\":71.64,\"feels_like\":72.54,\"temp_min\":71.64,\"temp_max\":"
        "71.64,\"pressure\":1017,\"sea_level\":1017,\"grnd_level\":1015,\"humidity\":86,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04n\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":10.04,\"deg\":172,\"gust\":21.74},"
        "\"visibility\":10000,\"pop\":0.26,\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-24 "
        "03:00:00\"},{\"dt\":1687586400,\"main\":{\"temp\":70.14,\"feels_like\":71.28,\"temp_min\":70.14,\"temp_max\":"
        "70.14,\"pressure\":1016,\"sea_level\":1016,\"grnd_level\":1014,\"humidity\":94,\"temp_kf\":0},\"weather\":[{"
        "\"id\":500,\"main\":\"Rain\",\"description\":\"light "
        "rain\",\"icon\":\"10n\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":5.82,\"deg\":203,\"gust\":12.26},"
        "\"visibility\":10000,\"pop\":0.73,\"rain\":{\"3h\":2.75},\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-24 "
        "06:00:00\"},{\"dt\":1687597200,\"main\":{\"temp\":69.01,\"feels_like\":70.16,\"temp_min\":69.01,\"temp_max\":"
        "69.01,\"pressure\":1015,\"sea_level\":1015,\"grnd_level\":1014,\"humidity\":97,\"temp_kf\":0},\"weather\":[{"
        "\"id\":500,\"main\":\"Rain\",\"description\":\"light "
        "rain\",\"icon\":\"10n\"}],\"clouds\":{\"all\":96},\"wind\":{\"speed\":6.08,\"deg\":184,\"gust\":9.91},"
        "\"visibility\":10000,\"pop\":0.7,\"rain\":{\"3h\":1.41},\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2023-06-24 "
        "09:00:00\"},{\"dt\":1687608000,\"main\":{\"temp\":70.88,\"feels_like\":72.03,\"temp_min\":70.88,\"temp_max\":"
        "70.88,\"pressure\":1016,\"sea_level\":1016,\"grnd_level\":1014,\"humidity\":93,\"temp_kf\":0},\"weather\":[{"
        "\"id\":500,\"main\":\"Rain\",\"description\":\"light "
        "rain\",\"icon\":\"10d\"}],\"clouds\":{\"all\":98},\"wind\":{\"speed\":7,\"deg\":174,\"gust\":9.55},"
        "\"visibility\":10000,\"pop\":0.59,\"rain\":{\"3h\":0.55},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-24 "
        "12:00:00\"},{\"dt\":1687618800,\"main\":{\"temp\":74.32,\"feels_like\":75.34,\"temp_min\":74.32,\"temp_max\":"
        "74.32,\"pressure\":1015,\"sea_level\":1015,\"grnd_level\":1013,\"humidity\":83,\"temp_kf\":0},\"weather\":[{"
        "\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast "
        "clouds\",\"icon\":\"04d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":7.43,\"deg\":178,\"gust\":10.76},"
        "\"visibility\":10000,\"pop\":0,\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-24 "
        "15:00:00\"},{\"dt\":1687629600,\"main\":{\"temp\":77.81,\"feels_like\":78.73,\"temp_min\":77.81,\"temp_max\":"
        "77.81,\"pressure\":1014,\"sea_level\":1014,\"grnd_level\":1012,\"humidity\":73,\"temp_kf\":0},\"weather\":[{"
        "\"id\":500,\"main\":\"Rain\",\"description\":\"light "
        "rain\",\"icon\":\"10d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":11.45,\"deg\":162,\"gust\":14.29},"
        "\"visibility\":7799,\"pop\":0.5,\"rain\":{\"3h\":1.53},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-24 "
        "18:00:00\"},{\"dt\":1687640400,\"main\":{\"temp\":73.94,\"feels_like\":75.04,\"temp_min\":73.94,\"temp_max\":"
        "73.94,\"pressure\":1013,\"sea_level\":1013,\"grnd_level\":1011,\"humidity\":85,\"temp_kf\":0},\"weather\":[{"
        "\"id\":501,\"main\":\"Rain\",\"description\":\"moderate "
        "rain\",\"icon\":\"10d\"}],\"clouds\":{\"all\":100},\"wind\":{\"speed\":12.73,\"deg\":165,\"gust\":16.89},"
        "\"visibility\":5253,\"pop\":0.74,\"rain\":{\"3h\":5.55},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2023-06-24 "
        "21:00:00\"}],\"city\":{\"id\":5110302,\"name\":\"Brooklyn\",\"coord\":{\"lat\":40.6526,\"lon\":-73.9497},"
        "\"country\":\"US\",\"population\":2300664,\"timezone\":-14400,\"sunrise\":1687166675,\"sunset\":1687220977}}";
    chestnut::MultiDayWeatherData weather;

    // WHEN
    auto ec = chestnut::MultiDayWeatherData::parse(data, weather);

    // THEN
    EXPECT_FALSE(ec);

    for (unsigned int i = 0; i < weather.getNumDays(); i++) {
        std::cout << "weather=" << weather.getDaily(i) << '\n';
    }
}
