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

find_package(Boost COMPONENTS system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(chat_server chat/chat_server.cpp)
target_link_libraries(chat_server ${Boost_LIBRARIES})

add_executable(chat_client chat/chat_client.cpp)
target_link_libraries(chat_client ${Boost_LIBRARIES})

add_executable(chat_client2 chat/chat_client.cpp)
target_link_libraries(chat_client2 ${Boost_LIBRARIES})

add_library(streamer project/src/streamer.cpp)
target_link_libraries(streamer ${OpenCV_LIBS})
target_link_libraries(streamer ${GST_LIBRARIES})
target_link_libraries(streamer Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network)

add_library(client project/src/client.cpp)
target_link_libraries(client ${OpenCV_LIBS})
target_link_libraries(client ${GST_LIBRARIES})
target_link_libraries(client Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network)

add_executable(main_test_server main_tests/main_test_server.cpp
        project/src/base_exception.cpp)
target_link_libraries(main_test_server streamer client)

add_executable(main_test_client main_tests/main_test_client.cpp
        project/src/base_exception.cpp)
target_link_libraries(main_test_client streamer client)

add_executable(main_qt
        main_qt.cpp
        qt/main_window/mainwindow.cpp
        qt/main_window/mainwindow.ui
        qt/role_settings_window/second_window.cpp
        qt/role_settings_window/second_window.ui
        qt/client_window/client_window.cpp
        qt/client_window/client_window.ui
        qt/video_reciever/video_receiver.cpp
        qt/my_qthread/my_qthread.cpp
        qt/streamer_window/streamer_window.cpp
        qt/streamer_window/streamer_window.ui
        project/src/base_exception.cpp
        )

add_executable(main_qt2
        main_qt.cpp
        qt/main_window/mainwindow.cpp
        qt/main_window/mainwindow.ui
        qt/role_settings_window/second_window.cpp
        qt/role_settings_window/second_window.ui
        qt/client_window/client_window.cpp
        qt/client_window/client_window.ui
        qt/video_reciever/video_receiver.cpp
        qt/my_qthread/my_qthread.cpp
        qt/streamer_window/streamer_window.cpp
        qt/streamer_window/streamer_window.ui
        project/src/base_exception.cpp
        )

target_include_directories(main_qt PUBLIC
        qt/main_window
        qt/role_settings_window
        qt/client_window
        qt/video_reciever
        qt/my_qthread
        qt/streamer_window
        )

target_include_directories(main_qt2 PUBLIC
        qt/main_window
        qt/role_settings_window
        qt/client_window
        qt/video_reciever
        qt/my_qthread
        qt/streamer_window
        )


target_link_libraries(main_qt streamer client)
target_link_libraries(main_qt2 streamer client)


#target_link_libraries(main_qt_client PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui ${OpenCV_LIBS})

include(cmake-build-debug/cmake_install.cmake)