cmake_minimum_required(VERSION 3.0.2)
project(jnp_zadanie1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -I/usr/include/boost -lboost_filesystem -lboost_system")

set(SOURCE_FILES
    opp.cc
	http_client.hpp)

add_executable(jnp_zadanie1 ${SOURCE_FILES})

target_link_libraries(jnp_zadanie1 boost_system boost_filesystem)
