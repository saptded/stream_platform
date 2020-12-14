#ifndef STREAM_PLATFORM_PROJECT_INCLUDE_CLIENT_HPP_
#define STREAM_PLATFORM_PROJECT_INCLUDE_CLIENT_HPP_

#include "streamer.hpp"

#include <QThread>
#include <QWidget>
#include <armadillo>
#include <glib.h>
#include <gst/gst.h>
#include <sstream>
#include <string>
#include <thread>
#include <QLabel>

namespace sp {
class Client {
 public:
    explicit Client(std::string nick = "anon");
    Client(const sp::Client &client);
    Client &operator=(const Client& client);
    void start_watch();
    void connect_to_server();
    void get_link(std::string data);
    void video_recieve(QLabel *label);
    void cap_act();
    int get_server_port();

    void audio_recieve();
    int server_port;
    std::string server_ip;
 private:
    cv::VideoCapture cap;
    cv::Mat frame;
    std::string nickname;
};

//class ClientWidget : public QWidget {
//    class VideoWorker : public QThread {
//
//    };

// public:
//    ClientWidget();
//
// private:
//};

}  // namespace sp

#endif  // STREAM_PLATFORM_PROJECT_INCLUDE_CLIENT_HPP_
