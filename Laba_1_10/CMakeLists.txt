cmake_minimum_required(VERSION 3.28)

project(Laba_1_10)

add_library(functions STATIC functions.c)

#find_package(ICU REQUIRED COMPONENTS uc i18n io)

add_executable(Laba_1_10 main.c)

#target_include_directories(Laba_1_10 PRIVATE ${ICU_INCLUDE_DIRS})

target_link_libraries(Laba_1_10 functions)