//
// Created by saptded on 15.12.2020.
//

#include "my_qthread.h"
#include <client.hpp>

AudioQThread::AudioQThread(sp::Client &client) : _client(client) {}
void AudioQThread::run() {
    _client.audio_recieve();
}
void AudioQThread::stop_gst_audio_loop() {
    g_main_loop_quit(_client.loop);
    g_main_loop_unref(_client.loop);
}
