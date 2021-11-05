cmake_minimum_required(VERSION 3.10)

project(Vazel)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

set(SRCS
    ./entity/entity.cpp
)

add_library(${PROJECT_NAME} ${SRCS})
