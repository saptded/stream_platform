#include "client.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace sp {
Client::Client() {
//    std::cout << "nickname: ";
//    std::cin >> nickname;
//    std::cout << "server_ip: ";
//    std::cin >> server_ip;
//    std::cout << "server_port: ";
//    std::cin >> server_port;
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

    int connect_res = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connect_res == -1) {
        throw "connect error";
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
void Client::start_watching() {
    cv::VideoCapture cap("udpsrc port=" + std::to_string(server_port) + " "
                             "! application/x-rtp, payload = 26, clock-rate=90000, encoding-name=JPEG "
                             "! rtpjpegdepay "
                             "! jpegdec "
                             "! videoconvert "
                             "! appsink",
                         cv::CAP_GSTREAMER);

    if (!cap.isOpened()) {
        throw "no signal";
    }

    cv::Mat frame;

    while (true) {
        cap >> frame;
        imshow("cap", frame);

        if (cv::waitKey(1) == 27) {
            break;
        }
    }
}

}  // namespace sp
