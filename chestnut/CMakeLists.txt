add_executable(chestnut
    chestnut.m.cpp
)
target_include_directories(chestnut PUBLIC .)
target_link_libraries(chestnut pthread)

install(TARGETS chestnut DESTINATION /opt/bin)
