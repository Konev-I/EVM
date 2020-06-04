cmake_minimum_required(VERSION 2.8)
project(arkanoid)

set(OpenCV_DIR "C:\\OpenCV2\\build\\install")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -g")

set(SOURCE_FILES
        sources/main.cpp
        )

include_directories(headers)

add_executable(arkanoid ${SOURCE_FILES} headers/message_queue.h
        headers/image.h
        headers/motion_detector.h headers/window.h
        headers/arkanoid.h headers/array_2d.h headers/timer.h sources/timer.cpp)
target_link_libraries(arkanoid ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
