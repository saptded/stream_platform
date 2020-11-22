cmake_minimum_required(VERSION 3.17)
project(Stream_platform)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(GST REQUIRED gstreamer-1.0)
#include_directories(${GST_INCLUDE_DIRS})
#add_definitions(${GST_DEFINITIONS})

add_executable(Stream_platform main.cpp)
target_link_libraries(Stream_platform ${OpenCV_LIBS})
#target_link_libraries(Stream_platform ${GST_LIBRARIES})

add_executable(client client.cpp)
target_link_libraries(client ${OpenCV_LIBS})
