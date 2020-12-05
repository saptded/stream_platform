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
#include <thread>
#include <mutex>

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
    void get_camera_settings();
    void start_stream();

 private:
    int port;
    Stgs settings;
    std::string streamer_nickname;
    int max_clients_amount;
    std::vector<Data_client> clients;
    std::vector<cv::VideoWriter> video_ports;
    void create_video_port(std::string client_ip);
};
}

#endif  // STREAM_PLATFORM_PROJECT_INCLUDE_STREAMER_H_
