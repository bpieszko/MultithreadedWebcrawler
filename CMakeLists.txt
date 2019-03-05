cmake_minimum_required(VERSION 3.13.3)

set(PROJECT_NAME MultithreadedWebcrawler)

project(${PROJECT_NAME})

find_package(Threads)
set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include ${CURL_INCLUDE_DIR})

file(GLOB SOURCES source/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${CURL_LIBRARIES})