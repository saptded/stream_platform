#include "client.hpp"

#include <QImage>
#include <QThread>
#include <QWidget>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <QLabel>
#include <QMainWindow>
#include "base_exception.h"


namespace sp {
Client::Client(std::string nick) : nickname(std::move(nick)) {}

Client::Client(const sp::Client &client)
    : nickname(client.nickname), server_port(client.server_port), server_ip(client.server_ip) {}

Client &Client::operator=(const Client &client) {
    nickname = client.nickname;
    server_port = client.server_port;
    server_ip = client.server_ip;

    return *this;
}

void Client::connect_to_server() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        throw "socket error";
    }

    int port = server_port;
    std::string ip_address = server_ip;

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ip_address.c_str(), &hint.sin_addr);

    int connect_res = connect(sock, (sockaddr *)&hint, sizeof(hint));
    if (connect_res == -1) {
        throw BaseException("connect error");
    }

    char buf[4096];
    memset(buf, 0, 4096);

    int sendRes = send(sock, nickname.c_str(), nickname.size(), 0);
    if (sendRes == -1) {
        std::cout << "send error" << std::endl;
    }

    int bytesReceived = recv(sock, buf, 4096, 0);
    if (bytesReceived == -1) {
        std::cout << "recv error" << std::endl;
    } else {
        std::cout << std::string(buf, bytesReceived) << std::endl;
    }

    close(sock);
}

void Client::cap_act() {
    cap = cv::VideoCapture("udpsrc port=" + std::to_string(server_port) +
                               " "
                               "! application/x-rtp, payload = 26, clock-rate=90000, encoding-name=JPEG "
                               "! rtpjpegdepay "
                               "! jpegdec "
                               "! videoconvert "
                               "! appsink",
                           cv::CAP_GSTREAMER);

    if (!cap.isOpened()) {
        throw BaseException("no signal");
    }
}

void Client::video_recieve(QLabel *label) {
    cap >> frame;

        label->setFixedHeight(720);
        label->setFixedWidth(1280);

        cv::resize(frame, frame, cv::Size(1280, 720));

        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        label->setPixmap(QPixmap::fromImage(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888)));
}
void Client::audio_recieve() {
    gst_init(nullptr, nullptr);

    loop = g_main_loop_new(nullptr, false);

    std::string prt = std::to_string(server_port + 1);
    char *c_port = const_cast<char *>(prt.c_str());

    gchar *gst_pipeline = g_strconcat("udpsrc port=", c_port, " ",
                                      "! application/x-rtp,"
                                      "media=(string)audio,"
                                      "clock-rate=(int)22000,"
                                      "width=16, height=16,"
                                      "encoding-name=(string)L16, encoding-params=(string)1,"
                                      "channels=(int)1, channel-positions=(int)1,"
                                      "payload=(int)96 "
                                      "! rtpL16depay "
                                      "! audioconvert "
                                      "! volume volume=1 "
                                      "! level "
                                      "! autoaudiosink",
                                      nullptr);

    gchar *descr = g_strdup(gst_pipeline);

    GError *error = nullptr;
    GstElement *pipeline = gst_parse_launch(descr, &error);

    if (error) {
        g_print("could not construct pipeline: %s\n", error->message);
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        g_clear_error(&error);
        g_main_loop_unref(loop);
        g_free(descr);
        throw "pipeline error";
    }

    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);

    g_main_loop_run(loop);

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_clear_error(&error);
    g_main_loop_unref(loop);
    g_free(descr);
}

void Client::stop_gst_audio_loop() {
    g_main_loop_quit(loop);
    g_main_loop_unref(loop);
}

void Client::get_link(std::string data) {
    this->server_port = 0;
    this->server_ip.clear();

    std::string dec;
    int i = 0;

    for (auto d : data) {
        if (d == 'a') {
            dec.push_back('0');
        }
        if (d == 'r') {
            dec.push_back('1');
        }
        if (d == 's') {
            dec.push_back('2');
        }
        if (d == 'f') {
            dec.push_back('3');
        }
        if (d == 'o') {
            dec.push_back('4');
        }
        if (d == 'p') {
            dec.push_back('5');
        }
        if (d == 'd') {
            dec.push_back('6');
        }
        if (d == 'k') {
            dec.push_back('7');
        }
        if (d == 'j') {
            dec.push_back('8');
        }
        if (d == 'g') {
            dec.push_back('9');
        }
        if (d == 'z') {
            dec.push_back('.');
        }
        if (d == '/') {
            if (i == 1) {
                break;
            }
            i++;
            dec.push_back(' ');
        }
    }

    std::string port;
    for (auto d : dec) {
        if (isspace(d)) {
            dec.erase(0, dec.find(d) + 1);
            break;
        }
        server_ip.push_back(d);
    }
    for (auto d : dec) {
        if (isdigit(d)) {
            port.push_back(d);
        }
    }

    if (!port.empty()) {
        server_port = stoi(port);
    }
}

int Client::get_server_port() { return server_port; }

}  // namespace sp
