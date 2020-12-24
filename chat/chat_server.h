#ifndef STREAM_PLATFORM_CHAT_SERVER_H
#define STREAM_PLATFORM_CHAT_SERVER_H

#include <ctime>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <algorithm>
#include <iomanip>
#include <array>
#include <memory>
#include <boost/asio.hpp>
#include "protocol.h"

#include <QString>
#include <QObject>

std::string getTimestamp();

class workerThread {
 public:
    static void run(std::shared_ptr<boost::asio::io_service> io_service);

 private:
    static std::mutex m;
};


class participant {
 public:
    virtual ~participant() = default;
    virtual void onMessage(std::array<char, MAX_IP_PACK_SIZE> & msg) = 0;
};

class chatRoom : public QObject {
    Q_OBJECT
 public:
    chatRoom() : QObject() {};
    virtual ~chatRoom() {};

    void enter(std::shared_ptr<participant> participant, const std::string & nickname);
    void leave(std::shared_ptr<participant> participant);
    void broadcast(std::array<char, MAX_IP_PACK_SIZE>& msg, std::shared_ptr<participant> participant);
    std::string getNickname(std::shared_ptr<participant> participant);

 signals:
    void show_message(const QString &msg);

 private:
    enum { max_recent_msgs = 100 };
    std::unordered_set<std::shared_ptr<participant>> participants_;
    std::unordered_map<std::shared_ptr<participant>, std::string> name_table_;
    std::deque<std::array<char, MAX_IP_PACK_SIZE>> recent_msgs_;
};

class personInRoom: public participant,
                    public std::enable_shared_from_this<personInRoom> {
 public:
    personInRoom(boost::asio::io_service& io_service,
                 boost::asio::io_service::strand& strand, chatRoom& room);
    boost::asio::ip::tcp::socket& socket();
    void start();
    void onMessage(std::array<char, MAX_IP_PACK_SIZE>& msg);

 private:
    void nicknameHandler(const boost::system::error_code& error);
    void readHandler(const boost::system::error_code& error);
    void writeHandler(const boost::system::error_code& error);

    boost::asio::ip::tcp::socket socket_;
    boost::asio::io_service::strand& strand_;
    chatRoom& room_;
    std::array<char, MAX_NICKNAME> nickname_{};
    std::array<char, MAX_IP_PACK_SIZE> read_msg_{};
    std::deque<std::array<char, MAX_IP_PACK_SIZE> > write_msgs_;
};

class server {
 public:
    server(boost::asio::io_service& io_service,
           boost::asio::io_service::strand& strand,
           const boost::asio::ip::tcp::endpoint& endpoint);

    chatRoom room_;
 private:
    void run();

    void onAccept(std::shared_ptr<personInRoom> new_participant, const boost::system::error_code& error);
    boost::asio::io_service& io_service_;
    boost::asio::io_service::strand& strand_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif  // STREAM_PLATFORM_CHAT_SERVER_H
