cmake_minimum_required(VERSION 2.8)
project(jnp_zadanie1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -O2 -Wall -Wunused -Wshadow -std=c++11 -I/usr/include/boost -L/usr/lib/x86_64-linux-gnu -lboost_thread -lboost_filesystem -lboost_system -lboost_regex -lpthread -lssl -lcrypto -lg")

set(SOURCE_FILES
    opp.cc)

add_executable(jnp_zadanie1 ${SOURCE_FILES})

#link_libraries(jnp_zadanie1 -lmicrohttpd -lmongoclient -lboost_thread -lboost_filesystem -lboost_system -lboost_regex -lpthread -lssl -lcrypto -lgnutls -lg)
