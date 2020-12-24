#include "streamer_window.h"

#include "./ui_streamer_window.h"
#include "video_receiver.h"

streamer_window::streamer_window(sp::Streamer& streamer, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::streamer_window), _streamer(streamer) {
    _parent = parent;

    sp::Client host_video("host");

    host_video.get_link(_streamer.create_link());

    ui->setupUi(this);

    _streamer.get_camera_settings();

    getting_usrs = std::thread(&sp::Streamer::getting_users, &_streamer);
    streaming = std::thread(&sp::Streamer::start_video_stream, &_streamer);

    //    getting_usrs.detach();
    //    streaming.detach();

    ui->video_label = new VideoReceiver(host_video, this);
    ui->video_label->setGeometry(100, 50, 1280, 720);

    connect(ui->disconnect_button, &QPushButton::clicked, this, &streamer_window::disconnect_button);
}

streamer_window::~streamer_window() {
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
