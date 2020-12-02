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
    int server_port = 0;
    std::string nickname;
    std::string server_ip;
};
}  // namespace sp

#endif  // STREAM_PLATFORM_PROJECT_INCLUDE_CLIENT_HPP_
