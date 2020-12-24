#include "client.hpp"
#include "streamer.hpp"
#include <QApplication>



int main() {
    sp::Streamer streamer("aaaa");

    streamer.create_link();

    streamer.get_camera_settings();

    std::thread gu(&sp::Streamer::getting_users, &streamer);
    std::thread ss(&sp::Streamer::start_video_stream, &streamer);


    gu.join();
    ss.join();

    return 0;
}