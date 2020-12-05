#include <streamer.hpp>
#include <utility>

std::mutex mtx;

namespace sp {
Streamer::Streamer() {
    std::cout << "nickname: ";
    std::cin >> streamer_nickname;
    std::cout << "port: ";
    std::cin >> port;
    std::cout << "max clients amount: ";
    std::cin >> max_clients_amount;
}

void Streamer::create_video_port(std::string client_ip) {
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

        cv::VideoWriter writer(gst_pipline, cv::CAP_GSTREAMER, 0, (int)settings.fps,
                               cv::Size(settings.width, settings.height), true);
        if (!writer.isOpened()) {
            throw "writer error";
        }

        mtx.lock();
        video_ports.push_back(writer);
        mtx.unlock();
}

void Streamer::getting_users() {
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket == -1) {
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

    sockaddr_in client_sock{};
    socklen_t client_sock_size = sizeof(client_sock);

    int i = 0;
    char buf[4096];
    Data_client client;
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    std::set<std::string> ips;

    while (i < max_clients_amount) {
        int clientSocket = accept(tcp_socket, (sockaddr*)&client_sock, &client_sock_size);

        if (clientSocket > 0) {
            memset(buf, 0, 4096);
            memset(host, 0, NI_MAXHOST);
            memset(service, 0, NI_MAXSERV);
            if (getnameinfo((sockaddr*)&client_sock, sizeof(client_sock), host, NI_MAXHOST, service, NI_MAXSERV, 0) !=
                0) {
                throw "getting ip error";
            }

            if (ips.count(std::string(host)) == 0) {
                int bytesReceived = recv(clientSocket, buf, 4096, 0);
                if (bytesReceived == -1) {
                    throw "recv error";
                }

                client.client_nickname = std::string(buf);
                client.ip = std::string(host);

                ips.insert(std::string(host));

                std::cout << client.client_nickname << " connected" << std::endl;

                i++;
            }

            clients.push_back(client);

            create_video_port(client.ip);

            send(clientSocket, "connected", 9, 0);

            close(clientSocket);
        }
    }

    close(tcp_socket);
}

void Streamer::get_camera_settings() {
    cv::VideoCapture cap(0);
    settings.width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    settings.height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    settings.fps = cap.get(cv::CAP_PROP_FPS);
}

void Streamer::start_stream() {

    cv::VideoCapture cap(0);
    cv::Mat frame;

    if (!cap.isOpened()) {
        throw "camera error";
    }

    create_video_port("fake_ip");

//    get_camera_settings(cap);

//    std::vector<cv::VideoWriter> clients_ports = create_ports();

    while (true) {
        cap >> frame;

        for (auto video_port : video_ports) {
            video_port << frame;
        }

//        std::cout << video_ports.size();

//        if (cv::waitKey(1) == 27) {
//            break;
//        }
    }
}

}  // namespace sp
