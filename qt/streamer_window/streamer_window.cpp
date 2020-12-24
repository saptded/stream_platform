#include "streamer_window.h"

#include "./ui_streamer_window.h"
#include "video_receiver.h"
#include <string>
#include <iostream>
#include <list>
#include <unordered_set>
#include <memory>
#include <thread>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "protocol.h"
#include "chat_server.h"

std::mutex workerThread::m;

streamer_window::streamer_window(sp::Streamer& streamer, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::streamer_window), _streamer(streamer) {
    _parent = parent;

    sp::Client host_video("host");

    host_video.get_link(_streamer.create_link());

    ui->setupUi(this);

    _streamer.get_camera_settings();

    getting_usrs = std::thread(&sp::Streamer::getting_users, &_streamer);
    streaming = std::thread(&sp::Streamer::start_video_stream, &_streamer);


    ui->video_label = new VideoReceiver(host_video, this);
    ui->video_label->setGeometry(100, 50, 1280, 720);

    connect(ui->disconnect_button, &QPushButton::clicked, this, &streamer_window::disconnect_button);

    thread_chat_server = std::thread(&streamer_window::run_server_chat, this);
}

void streamer_window::run_server_chat() {
        try {
        std::shared_ptr<boost::asio::io_service> io_service(new boost::asio::io_service);
        boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*io_service));
        boost::shared_ptr<boost::asio::io_service::strand> strand(new boost::asio::io_service::strand(*io_service));

        std::cout << "[" << std::this_thread::get_id() << "]" << "server starts" << std::endl;

        std::list < std::shared_ptr < server >> servers;

        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), std::atoi("8200"));
        std::shared_ptr<server> a_server(new server(*io_service, *strand, endpoint));
        servers.push_back(a_server);


        boost::thread_group workers;
        for (int i = 0; i < 1; ++i) {
            boost::thread * t = new boost::thread{ boost::bind(&workerThread::run, io_service) };
            workers.add_thread(t);
        }

        workers.join_all();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

streamer_window::~streamer_window() {
    thread_chat_server.join();
    getting_usrs.join();
    streaming.join();
    delete ui;
}

void streamer_window::disconnect_button() {
    _streamer.stop_run();
    _streamer.stop_audio();
    _parent->show();
    delete this;
}
