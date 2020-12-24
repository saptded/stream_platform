#ifndef STREAM_PLATFORM_CHAT_CLIENT_H
#define STREAM_PLATFORM_CHAT_CLIENT_H

class client {
 public:
    client(const std::array<char, MAX_NICKNAME>& nickname,
           boost::asio::io_service& io_service,
           boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

    void write(const std::array<char, MAX_IP_PACK_SIZE>& msg);
    void close();

 private:
    void onConnect(const boost::system::error_code& error);
    void readHandler(const boost::system::error_code& error);
    void writeImpl(std::array<char, MAX_IP_PACK_SIZE> msg);
    void writeHandler(const boost::system::error_code& error);
    void closeImpl();

    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::socket socket_;
    std::array<char, MAX_IP_PACK_SIZE> read_msg_{};
    std::deque<std::array<char, MAX_IP_PACK_SIZE>> write_msgs_;
    std::array<char, MAX_NICKNAME> nickname_{};
};

#endif  // STREAM_PLATFORM_CHAT_CLIENT_H
