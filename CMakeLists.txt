cmake_minimum_required(VERSION 3.17)
project(stream_platform)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS -pthread)

include_directories(project/include)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)
include_directories(${GST_INCLUDE_DIRS})
add_definitions(${GST_DEFINITIONS})

add_executable(streamer project/src/streamer.cpp)
target_link_libraries(streamer ${OpenCV_LIBS})
target_link_libraries(streamer ${GST_LIBRARIES})

add_executable(client project/src/client.cpp)
target_link_libraries(client ${OpenCV_LIBS})
target_link_libraries(client ${GST_LIBRARIES})

add_executable(main_test_server main_tests/main_test_server.cpp project/src/streamer.cpp project/src/client.cpp)
target_link_libraries(main_test_server ${OpenCV_LIBS})
target_link_libraries(main_test_server ${GST_LIBRARIES})

add_executable(main_test_client main_tests/main_test_client.cpp project/src/streamer.cpp project/src/client.cpp)
target_link_libraries(main_test_client ${OpenCV_LIBS})
target_link_libraries(main_test_client ${GST_LIBRARIES})

