#include <streamer.hpp>
#include <utility>

namespace sp {
Streamer::Streamer() {
    std::cout << "nickname: ";
    std::cin >> nickname;
    std::cout << "port: ";
    std::cin >> port;
    std::cout << "max clients amount: ";
    std::cin >> max_clients_amount;
}

std::vector<cv::VideoWriter> Streamer::create_ports() {
    std::vector<cv::VideoWriter> ports;

    for (const auto& client_ip : clients_ip) {
        std::string gst_pipline = "appsrc "
                                  "! videoconvert "
                                  "! video/x-raw, format=YUY2,"
                                  "width = " + std::to_string(settings.width) + ","
                                  "height = " + std::to_string(settings.height) + ","
                                  "framerate = " + std::to_string((int)settings.fps) + "/1 "
                                  "! jpegenc "
                                  "! rtpjpegpay "
                                  "! udpsink "
                                  "host = " + client_ip + " "
                                  "port = " + std::to_string(port);

        cv::VideoWriter writer(gst_pipline, cv::CAP_GSTREAMER, 0,
                               (int)settings.fps,
                               cv::Size(settings.width, settings.height),
                               true);
        if (!writer.isOpened()) {
            throw "writer error";
        }

        ports.push_back(writer);
    }

    return ports;
}

void Streamer::start_stream() {
    cv::VideoCapture cap(0);
    cv::Mat frame;

    if (!cap.isOpened()) {
        throw "camera error";
    }

    get_camera_settings(cap);

    std::vector<cv::VideoWriter> clients_ports = create_ports();

    while (true) {
        cap >> frame;

        for (auto client_port : clients_ports) {
            client_port << frame;
        }

        if (cv::waitKey(1) == 27) {
            break;
        }
    }

}

void Streamer::getting_users() {
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket == -1)
    {
        throw "socket error";
    }

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(tcp_socket, (sockaddr*)&hint, sizeof(hint)) == -1) {
        throw "bind error";
    }

    listen(tcp_socket, SOMAXCONN);

    sockaddr_in client{};
    socklen_t clientSize = sizeof(client);

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    for (int i = 0; i < max_clients_amount; i++) {
        int clientSocket = accept(tcp_socket, (sockaddr*)&client, &clientSize);

        if (clientSocket > 0) {
            memset(host, 0, NI_MAXHOST);
            memset(service, 0, NI_MAXSERV);
            getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0);

            clients_ip.insert(host);

            send(clientSocket, "connected", 9, 0);

            close(clientSocket);
        }
    }

    close(tcp_socket);
}

void Streamer::get_camera_settings(const cv::VideoCapture& cap) {
    settings.width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    settings.height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    settings.fps = cap.get(cv::CAP_PROP_FPS);
}


}
