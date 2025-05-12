cmake_minimum_required(VERSION 3.28)

project(Laba_5_10)

add_library(functions STATIC functions.c)

add_executable(Laba_5_10 main.c)

target_link_libraries(Laba_5_10 functions)
target_link_libraries(Laba_5_10 m)