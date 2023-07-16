add_executable(chestnut
    chestnut.cpp
    chestnut.m.cpp
    config.cpp
    weather/weatherclient.cpp
    weather/weatherdata.cpp
    weather/weatherdatapublisher.cpp
)
target_include_directories(chestnut PUBLIC .)
target_link_libraries(chestnut pthread fwoopbasis fwoophttp fwoopbasis fwoopencoding fwoopobserve)

install(TARGETS chestnut DESTINATION /opt/bin)

include(GoogleTest)
add_executable(chestnut_test
    chestnut.g.cpp
    config.g.cpp
    weather/weatherclient.g.cpp
    weather/weatherdata.g.cpp
    weather/weatherdatapublisher.g.cpp

    chestnut.cpp
    config.cpp
    weather/weatherclient.cpp
    weather/weatherdata.cpp
    weather/weatherdatapublisher.cpp
)
target_include_directories(chestnut_test PUBLIC .)
target_link_libraries(chestnut_test fwoopobserve fwoopbasis fwoophttp fwoopbasis fwoopencoding fwoopobserve gtest gtest_main pthread)

gtest_add_tests(TARGET chestnut_test)
