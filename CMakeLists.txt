cmake_minimum_required(VERSION 3.17)
project(stream_platform)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

SET(CMAKE_CXX_FLAGS -pthread)

include_directories(project/include)

find_package(Qt5 COMPONENTS Gui Core Widgets Network REQUIRED)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)
include_directories(${GST_INCLUDE_DIRS})
add_definitions(${GST_DEFINITIONS})

add_library(streamer project/src/streamer.cpp)
target_link_libraries(streamer ${OpenCV_LIBS})
target_link_libraries(streamer ${GST_LIBRARIES})
target_link_libraries(streamer Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network)

add_library(client project/src/client.cpp)
target_link_libraries(client ${OpenCV_LIBS})
target_link_libraries(client ${GST_LIBRARIES})
target_link_libraries(client Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network)

add_executable(main_test_server main_tests/main_test_server.cpp)
target_link_libraries(main_test_server streamer client)

add_executable(main_test_client main_tests/main_test_client.cpp)
target_link_libraries(main_test_client streamer client)

add_executable(main_qt
        main_qt.cpp
        main_window/mainwindow.cpp
        main_window/mainwindow.ui
        second_window/second_window.cpp
        second_window/second_window.ui
        )
target_include_directories(main_qt PUBLIC
        main_window
        second_window
        )

target_link_libraries(main_qt streamer client)


#target_link_libraries(main_qt_client PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui ${OpenCV_LIBS})

include(cmake-build-debug/cmake_install.cmake)