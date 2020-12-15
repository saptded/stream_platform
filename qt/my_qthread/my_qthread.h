//
// Created by saptded on 15.12.2020.
//

#ifndef STREAM_PLATFORM_MY_QTHREAD_MY_QTHREAD_H_
#define STREAM_PLATFORM_MY_QTHREAD_MY_QTHREAD_H_

#include <QThread>
#include <client.hpp>

class AudioQThread : public QThread {
 public:
    explicit AudioQThread(sp::Client &client);
    void run();
    void stop_gst_audio_loop();
 private:
    sp::Client _client;
};

#endif  // STREAM_PLATFORM_MY_QTHREAD_MY_QTHREAD_H_
