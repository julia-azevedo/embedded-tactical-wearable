# Onde encontrar o Zephyr e quais são seus arquivos C.

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(tactical_weareble)

target_sources(app PRIVATE src/main.c)
