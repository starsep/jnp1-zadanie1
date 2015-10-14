cmake_minimum_required(VERSION 2.8)
project(jnp_zadanie1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -O2 -Wall -Wunused -Wshadow -std=c++11 -I/usr/include/boost -L/usr/lib/x86_64-linux-gnu -lboost_regex")

set(SOURCE_FILES
    opp.cc)

add_executable(jnp_zadanie1 ${SOURCE_FILES})

