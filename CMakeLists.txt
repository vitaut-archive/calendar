cmake_minimum_required(VERSION 3.16)
project(CALENDAR)

add_executable(calendar calendar.cc src/format.cc)
target_include_directories(calendar PUBLIC include)
target_compile_features(calendar PUBLIC cxx_std_20)
target_compile_options(calendar PUBLIC -fcoroutines-ts)

add_executable(calendar-unicode calendar-unicode.cc src/format.cc)
target_include_directories(calendar-unicode PUBLIC include)
target_compile_features(calendar-unicode PUBLIC cxx_std_20)
target_compile_options(calendar-unicode PUBLIC -fcoroutines-ts)

