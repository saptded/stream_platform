#include <streamer.hpp>
#include <utility>

std::mutex mtx;

namespace sp {
Streamer::Streamer(std::string nick) : streamer_nickname(std::move(nick)) {}

void Streamer::create_audio_port(const std::string& client_ip, GMainLoop *loop) {
    gst_init(nullptr, nullptr);

    std::string prt = std::to_string(port + 1);
    char *c_port = const_cast<char *>(prt.c_str());
    char *c_host = const_cast<char *>(client_ip.c_str());

    gchar *gst_pipeline = g_strconcat("pulsesrc "
                                      "! audioconvert "
                                      "! audio/x-raw,"
                                      "channels=1,depth=16,width=16,rate=22000 "
                                      "! rtpL16pay "
                                      "! udpsink host=",
                                      c_host, " port=", c_port, nullptr);

    gchar *descr = g_strdup(gst_pipeline);

    GError *error = nullptr;
    GstElement *pipeline = gst_parse_launch(descr, &error);

    if (error) {
        g_print("could not construct pipeline: %s\n", error->message);
        g_main_loop_run(loop);
        gst_object_unref (pipeline);
        g_error_free(error);
        g_main_loop_unref (loop);
        g_free(descr);
        throw "audio pipeline error";
    }

    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);

    audio_loops.push_back(loop);

    loops_amount++;

    g_main_loop_run(loop);

    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);
    g_error_free(error);
    g_free(descr);
}

void Streamer::create_video_port(const std::string &client_ip) {
    std::string gst_pipline = "appsrc "
                              "! videoconvert "
                              "! video/x-raw, format=YUY2,"
                              "width = " +
                              std::to_string(settings.width) +
                              ","
                              "height = " +
                              std::to_string(settings.height) +
                              ","
                              "framerate = " +
                              std::to_string((int)settings.fps) +
                              "/1 "
                              "! jpegenc "
                              "! rtpjpegpay "
                              "! udpsink "
                              "host = " +
                              client_ip +
                              " "
                              "port = " +
                              std::to_string(port);

    mtx.lock();
    video_ports.emplace_back(gst_pipline, cv::CAP_GSTREAMER, 0, (int)settings.fps,
                             cv::Size(settings.width, settings.height), true);
    mtx.unlock();

    if (!video_ports.back().isOpened()) {
        throw "writer error";
    }
}

void Streamer::getting_users() {
    int tcp_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (tcp_socket == -1) {
        throw BaseException("socket error");
    }

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    try {
        if (bind(tcp_socket, (sockaddr *)&hint, sizeof(hint)) == -1) {
            throw BaseException("bind error");
        }
    } catch (BaseException &err) {
        getting_users();
        this->change_port();
    }


    listen(tcp_socket, SOMAXCONN);

    sockaddr_in client_sock{};
    socklen_t client_sock_size = sizeof(client_sock);

    int i = 0;
    std::string buf;
    Data_client client;
    std::set<std::string> ips;

    while (i < max_clients_amount) {
        int clientSocket = accept(tcp_socket, (sockaddr *)&client_sock, &client_sock_size);

        if (!run) {
            break;
        }

        if (clientSocket > 0) {
            buf.resize(4096);
            if (ips.count(inet_ntoa(client_sock.sin_addr)) == 0) {
                int bytesReceived = recv(clientSocket, buf.data(), buf.size(), 0);  // buf.data(), buf.size()
                if (bytesReceived == -1) {
                    throw BaseException("recv error");
                }
                buf.resize(bytesReceived);
                client.client_nickname = std::string(buf);
                client.ip = inet_ntoa(client_sock.sin_addr);

                ips.insert(inet_ntoa(client_sock.sin_addr));

                if (client.client_nickname == "host") {
                } else {
                    std::cout << client.client_nickname << " connected" << std::endl;
                }

                clients.push_back(client);

                create_video_port(client.ip);

                send(clientSocket, "connected", 10, 0);

                i++;
            } else {
                send(clientSocket, "you are already connected", 26, 0);
            }

            close(clientSocket);
        }
    }

    close(tcp_socket);
}

void Streamer::get_camera_settings() {
    cv::VideoCapture cap(cam_index);
    settings.width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    settings.height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    settings.fps = cap.get(cv::CAP_PROP_FPS);
}

void Streamer::start_video_stream() {
    std::thread video(&Streamer::video_send, this);
    std::thread audio(&Streamer::audio_send, this);


    video.join();
    audio.join();
}

void Streamer::video_send() {
    cv::VideoCapture cap(cam_index);
    cv::Mat frame;

    if (!cap.isOpened()) {
        throw BaseException("camera error");
    }

    while (true) {
        cap >> frame;

        for (auto video_port : video_ports) {
            video_port << frame;
        }
        if (!run) {
            break;
        }
    }
}

void Streamer::audio_send() {
    int size = clients.size();
    while (true) {
      if (clients.size() != size) {
          GMainLoop *lp = g_main_loop_new(nullptr, false);;
          audio_ports.emplace_back(&Streamer::create_audio_port, this, clients.back().ip, lp);
          size = clients.size();
      }
      if (!run) {
          break;
      }
    }
}

void Streamer::get_local_ip() {
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    QString str;

    for (int nIter = 0; nIter < list.count(); nIter++) {
        if (!list[nIter].isLoopback())
            if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol)
                str = list[nIter].toString();
        local_ip = str.toUtf8().constData();
        if (!local_ip.empty()) {
            break;
        }
    }

    if (local_ip.empty()) {
        local_ip = "127.0.0.1";
    }

}

std::string Streamer::create_link() {
    get_local_ip();
    std::string enc;
    std::string data = local_ip + " " + std::to_string(port);

    enc.push_back('.');
    for (auto d : data) {
        if (d == '0') {
            enc.push_back('a');
        }
        if (d == '1') {
            enc.push_back('r');
        }
        if (d == '2') {
            enc.push_back('s');
        }
        if (d == '3') {
            enc.push_back('f');
        }
        if (d == '4') {
            enc.push_back('o');
        }
        if (d == '5') {
            enc.push_back('p');
        }
        if (d == '6') {
            enc.push_back('d');
        }
        if (d == '7') {
            enc.push_back('k');
        }
        if (d == '8') {
            enc.push_back('j');
        }
        if (d == '9') {
            enc.push_back('g');
        }
        if (d == '.') {
            enc.push_back('z');
        }
        if (d == ' ') {
            enc.push_back('/');
        }
    }

    enc += "/hosthorn:" + streamer_nickname;

    std::cout << enc << std::endl;

    return enc;
}
Streamer::~Streamer() {
    for (auto & thread: audio_ports) {
        thread.join();
    }
}
void Streamer::set_cam_index(int index) {
    cam_index = index;
}
void Streamer::set_max_client_amount(int max) {
    max_clients_amount = max;
}

Streamer::Streamer(const sp::Streamer &streamer)
    : cam_index(streamer.cam_index),
      max_clients_amount(streamer.max_clients_amount),
      streamer_nickname(streamer.streamer_nickname),
      port(streamer.port), local_ip(streamer.local_ip) {}

void Streamer::change_port() {
    port++;
}

void Streamer::stop_audio() {
    for (int i = 0; i < loops_amount; i++) {
        g_main_loop_quit(audio_loops[i]);
        g_main_loop_unref(audio_loops[i]);
    }
}
void Streamer::stop_run() {
    run = false;
}
Streamer &Streamer::operator=(const Streamer &streamer) {
    port = streamer.port;
    local_ip = streamer.local_ip;
    cam_index = streamer.cam_index;
    streamer_nickname = streamer.streamer_nickname;
    max_clients_amount = streamer.max_clients_amount;

    return *this;
}

}  // namespace sp
