#include "client_window.h"

#include <utility>
#include "./ui_client_window.h"
#include "video_receiver.h"

client_window::client_window(sp::Client &client, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::client_window), _client(client)
{
    ui->setupUi(this);

    connect(ui->disconnect_button, &QPushButton::clicked, this, &client_window::disconnect_button);

    _parent = parent;

    ui->video_label = new VideoReceiver{_client, this};
    ui->video_label->setGeometry(100, 50, 1280, 720);

    audio = new AudioQThread(_client);

    audio->start();
}

client_window::~client_window()
{
    audio->wait();
    delete ui;
}

void client_window::disconnect_button() {
    audio->stop_gst_audio_loop();
//    delete audio;
    _parent->show();
    delete this;
}

