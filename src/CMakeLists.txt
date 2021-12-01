cmake_minimum_required(VERSION 3.10)

project(Vazel)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/"
)

set(CMAKE_BUILD_TYPE Debug)

set(SRCS
    ./UUID.cpp
    ./ecs/Entity/Entity.cpp
    ./ecs/Entity/EntityManager.cpp
    ./ecs/Components/Component.cpp
    ./ecs/Components/ComponentsManager.cpp
    ./ecs/System/System.cpp
    ./ecs/World/World.cpp
    ./core/App.cpp
    ./core/State.cpp
)

add_library(${PROJECT_NAME} ${SRCS})
