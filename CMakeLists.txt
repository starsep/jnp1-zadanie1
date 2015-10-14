#set(SOURCE_FILES
#    opp.cc)

cmake_minimum_required(VERSION 2.8)
project(jnp_zadanie1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -O2 -Wall -Wunused -Wshadow -std=c++11 -lboost_regex")

add_custom_target(opp COMMAND make -C ${jnp_zadanie1_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})