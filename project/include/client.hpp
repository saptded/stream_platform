#ifndef STREAM_PLATFORM_PROJECT_INCLUDE_CLIENT_HPP_
#define STREAM_PLATFORM_PROJECT_INCLUDE_CLIENT_HPP_

#include <string>

namespace sp {
class Client {
 public:
    Client();
    void start_watching();
    void connect_to_server();

 private:
    int server_port = 8080;
    std::string nickname = "Kamilya";
    std::string server_ip = "192.168.0.111";
};
}  // namespace sp

#endif  // STREAM_PLATFORM_PROJECT_INCLUDE_CLIENT_HPP_
