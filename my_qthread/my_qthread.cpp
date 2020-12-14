//
// Created by saptded on 15.12.2020.
//

#include "my_qthread.h"
#include <client.hpp>

AudioQThread::AudioQThread(sp::Client &client) : _client(client) {}
void AudioQThread::run() {
    _client.audio_recieve();
}
