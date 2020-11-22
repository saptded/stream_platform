add_library(video STATIC src/transceiver.cpp src/reciever.cpp)

target_link_libraries(video PUBLIC include)
find_package(OpenCV REQUIRED)
target_link_libraries(video PUBLIC ${OpenCV_LIBS})
