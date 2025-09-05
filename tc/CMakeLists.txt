cmake_minimum_required(VERSION 3.11)

project(client
LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS Core)

add_library(tcp_tc SHARED tcp_tc.cpp)

target_link_libraries(tcp_tc PUBLIC Qt5::Core)

target_include_directories(tcp_tc PUBLIC ${CMAKE_SOURCE_DIR}/inc)

set_target_properties(tcp_tc PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

add_executable(${PROJECT_NAME} client.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/inc)

target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Werror)

target_link_libraries(${PROJECT_NAME} tcp_tc)

add_executable(server server.cpp)

set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/inc)

target_compile_options(server PUBLIC -Wall -Werror)

target_link_libraries(server tcp_tc)