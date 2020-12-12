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
    Streamer();
    ~Streamer();
    void getting_users();
    void get_camera_settings();
    void start_video_stream();
    void create_link();

 private:
    int port = 8081;
    Stgs settings;
    std::string streamer_nickname;
    int max_clients_amount = 10;
    std::vector<Data_client> clients;
//    std::map<std::string, Client> clients;
    /*
     * clients[ip] = Client{
     *      nick
     *      std::thread{&Stramer... ip}
     * };
     *
     *
     */
    std::vector<cv::VideoWriter> video_ports;
    std::vector<std::thread> audio_ports;
    void video_send();
    void audio_send();
    std::string get_local_ip();
    void create_audio_port(const std::string& client_ip);
    void create_video_port(const std::string& client_ip);
};
}

#endif  // STREAM_PLATFORM_PROJECT_INCLUDE_STREAMER_H_
