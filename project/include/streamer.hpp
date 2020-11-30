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
    int width = 1280;
    int height = 720;
    double fps = 10;
};

namespace sp {
class Streamer {
 public:
    explicit Streamer();
    void start_stream();
    void getting_users();
    void get_camera_settings(const cv::VideoCapture& cap);

 private:
    int port;
    Stgs settings;
    std::string nickname;
    int max_clients_amount;
    std::set<std::string> clients_ip;
    std::vector<cv::VideoWriter> create_ports();
};
}

#endif  // STREAM_PLATFORM_PROJECT_INCLUDE_STREAMER_H_
