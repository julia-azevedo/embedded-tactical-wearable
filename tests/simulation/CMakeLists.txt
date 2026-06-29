cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(tactical_tests)

target_include_directories(app PRIVATE ../../src)

target_sources(app PRIVATE 
    src/main.c
    ../../src/tatico_events.c
    ../../src/tatico_settings.c
    ../../src/tatico_button.c
)