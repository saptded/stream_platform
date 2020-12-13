#ifndef STREAM_PLATFORM_PROJECT_INCLUDE_CLIENT_HPP_
#define STREAM_PLATFORM_PROJECT_INCLUDE_CLIENT_HPP_

#include <QThread>
#include <QWidget>
#include <glib.h>
#include <gst/gst.h>
#include <string>
#include <thread>
#include <sstream>

namespace sp {
class Client {
 public:
    Client(std::string nick);
    void start_watch();
    void connect_to_server();
    void get_link(std::string data);
    void video_recieve();
    int get_server_port();

    int server_port;
    std::string server_ip;
 private:
    std::string nickname;
    void audio_recieve();
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
