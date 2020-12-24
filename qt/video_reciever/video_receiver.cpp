//
// Created by saptded on 12.12.2020.
//

#include <chrono>

using namespace std::chrono_literals;

#include "client.hpp"
#include "video_receiver.h"
#include <base_exception.h>
#include "my_qthread.h"

VideoReceiver::VideoReceiver(sp::Client &client, QWidget *parent) : QLabel(parent), _client(client){

    connect(&_timer, &QTimer::timeout, this, &VideoReceiver::update_image);
    _timer.start(15ms);

    _parent = parent;

    _client.connect_to_server();

    _client.cap_act();
}

void VideoReceiver::update_image() {
    _client.video_recieve(this);


}
