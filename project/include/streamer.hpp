#ifndef STREAM_PLATFORM_PROJECT_INCLUDE_STREAMER_H_
#define STREAM_PLATFORM_PROJECT_INCLUDE_STREAMER_H_

#include <arpa/inet.h>
#include <cstring>
#include <glib.h>
#include <gst/gst.h>
#include <iostream>
#include <mutex>
#include <netdb.h>
#include <opencv2/opencv.hpp>
#include <set>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QNetworkInterface>
#include <cmath>
#include "base_exception.h"

struct Stgs {
    int width = 0;
    int height = 0;
    double fps = 0;
};

struct Data_client {
    std::string ip;
    std::string client_nickname;
};

struct Client {
    std::string nick;
    std::thread audio;
    std::thread chat;
};

namespace sp {
class Streamer {
 public:
    Streamer(std::string nick = "master");
    ~Streamer();
    Streamer(const sp::Streamer &streamer);
    Streamer &operator=(const Streamer& streamer);
    void getting_users();
    void get_camera_settings();
    void start_video_stream();
    std::string create_link();
    void set_cam_index(int index);
    void set_max_client_amount(int max);
    void change_port();
    void stop_audio();

    void stop_run();


 private:
    std::string local_ip;
    bool run = true;
    int cam_index = 0;
    int port = 8081;
    Stgs settings;
    std::string streamer_nickname;
    int max_clients_amount = 10;
    std::vector<Data_client> clients;
    std::vector<cv::VideoWriter> video_ports;
    std::vector<std::thread> audio_ports;
    std::vector<GMainLoop*> audio_loops;
    int loops_amount = 0;
    void video_send();
    void audio_send();
    void get_local_ip();
    void create_audio_port(const std::string& client_ip, GMainLoop *loop);
    void create_video_port(const std::string& client_ip);
};
}

#endif  // STREAM_PLATFORM_PROJECT_INCLUDE_STREAMER_H_
