cmake_minimum_required(VERSION 3.5)

project(ampl)

set(CMAKE_CXX_COMPILER clang++)

add_compile_options(-std=c++17 -stdlib=libc++ -Wall -Werror -g -O2)

file(GLOB_RECURSE src src/ampl/*.cpp)
add_executable(ampl ${src} src/main.cpp)
target_include_directories(ampl PUBLIC src/)

add_executable(tests ${src} src/tests.cpp)
target_include_directories(tests PUBLIC src/)

install(FILES build/ampl DESTINATION bin)
