#ifndef STREAM_PLATFORM_PROJECT_INCLUDE_STREAMER_H_
#define STREAM_PLATFORM_PROJECT_INCLUDE_STREAMER_H_

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <set>

struct Stgs {
    int width = 0;
    int height = 0;
    double fps = 0;
};

struct Data_client {
    std::string ip;
    std::string client_nickname;
};

namespace sp {
class Streamer {
 public:
    Streamer();
    void getting_users();
    void get_camera_settings(const cv::VideoCapture& cap);
    void start_stream();

 private:
    int port;
    Stgs settings;
    std::string streamer_nickname;
    int max_clients_amount;
    std::vector<Data_client> clients;
    std::vector<cv::VideoWriter> create_ports();
};
}

#endif  // STREAM_PLATFORM_PROJECT_INCLUDE_STREAMER_H_
