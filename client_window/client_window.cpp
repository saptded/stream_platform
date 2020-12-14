#include "client_window.h"

#include <utility>
#include "./ui_client_window.h"
#include "video_receiver.h"

client_window::client_window(sp::Client &client, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::client_window), _client(client)
{
    ui->setupUi(this);

    ui->label = new VideoReceiver{_client, this};

    audio = new AudioQThread(_client);

    audio->start();

}

client_window::~client_window()
{
    delete ui;
    audio->wait();
}
