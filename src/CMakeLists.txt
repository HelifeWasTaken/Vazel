cmake_minimum_required(VERSION 3.10)

project(Vazel)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/"
)

set(CMAKE_BUILD_TYPE Debug)

set(SRCS
    ./UUID.cpp
    ./Entity/Entity.cpp
    ./Entity/EntityManager.cpp
    ./Components/Component.cpp
    ./Components/ComponentsManager.cpp
    ./System/System.cpp
    ./World/World.cpp
)

add_library(${PROJECT_NAME} ${SRCS})
