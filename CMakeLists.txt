cmake_minimum_required(VERSION 3.16)
project(CALENDAR)

add_executable(calendar calendar.cc)
target_include_directories(calendar PUBLIC include)
target_compile_features(calendar PUBLIC cxx_std_11)