cmake_minimum_required(VERSION 3.10)

project(Vazel VERSION 1.0)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

set(SRCS
    ./main.cpp
    ./src/entity/entity.cpp
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(vazel ${SRCS})
