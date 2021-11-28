cmake_minimum_required(VERSION 3.10)

project(vazel_tests VERSION 1.0)

set(SRCS
    ./Entity/test_Entity.cpp
    ./Entity/test_EntityManager.cpp
    ./Components/test_ComponentsManager.cpp
    ./System/test_System.cpp
)


add_executable(${PROJECT_NAME} ${SRCS})

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/"
)
target_link_libraries(${PROJECT_NAME}
    gtest_main
    Vazel
)

add_test(NAME vazel_tests COMMAND ${PROJECT_NAME})
