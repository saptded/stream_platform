#include "streamer.hpp"
#include "client.hpp"

int main() {

    sp::Streamer streamer;

    streamer.get_camera_settings();

    std::thread gu(&sp::Streamer::getting_users, &streamer);
    std::cout << gu.joinable() << std::endl;

    std::thread ss(&sp::Streamer::start_stream, &streamer);

//    std::cout << gu.joinable() << std::endl;
    gu.join();
//    std::cout << gu.joinable() << std::endl;
    ss.join();

//    streamer.getting_users();
//
//    streamer.start_stream();


}