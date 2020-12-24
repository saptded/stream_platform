#include <deque>
#include <array>
#include <thread>
#include <iostream>
#include <cstring>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "protocol.h"

using boost::asio::ip::tcp;

class client_chat {
 public:
    client_chat(const std::array<char, MAX_NICKNAME>& nickname,
           boost::asio::io_service& io_service,
           tcp::resolver::iterator endpoint_iterator) :
           io_service_(io_service), socket_(io_service) {

        strcpy(nickname_.data(), nickname.data());
        memset(read_msg_.data(), '\0', MAX_IP_PACK_SIZE);
        boost::asio::async_connect(socket_, endpoint_iterator, boost::bind(&client_chat::onConnect, this, _1));
    }

    void write(const std::array<char, MAX_IP_PACK_SIZE>& msg) {
        io_service_.post(boost::bind(&client_chat::writeImpl, this, msg));
    }

    void close() {
        io_service_.post(boost::bind(&client_chat::closeImpl, this));
    }

 private:
    void onConnect(const boost::system::error_code& error) {
        if (!error) {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(nickname_, nickname_.size()),
                                     boost::bind(&client_chat::readHandler, this, _1));
        }
    }

    void readHandler(const boost::system::error_code& error) {
        std::cout << read_msg_.data() << std::endl;
        if (!error) {
            boost::asio::async_read(socket_,
                                    boost::asio::buffer(read_msg_, read_msg_.size()),
                                    boost::bind(&client_chat::readHandler, this, _1));
        } else {
            closeImpl();
        }
    }

    void writeImpl(std::array<char, MAX_IP_PACK_SIZE> msg) {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress) {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(write_msgs_.front(), write_msgs_.front().size()),
                                     boost::bind(&client_chat::writeHandler, this, _1));
        }
    }

    void writeHandler(const boost::system::error_code& error) {
        if (!error) {
            write_msgs_.pop_front();
            if (!write_msgs_.empty()) {
                boost::asio::async_write(socket_,
                                         boost::asio::buffer(write_msgs_.front(), write_msgs_.front().size()),
                                         boost::bind(&client_chat::writeHandler, this, _1));
            }
        } else {
            closeImpl();
        }
    }

    void closeImpl() {
        socket_.close();
    }

    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    std::array<char, MAX_IP_PACK_SIZE> read_msg_;
    std::deque<std::array<char, MAX_IP_PACK_SIZE>> write_msgs_;
    std::array<char, MAX_NICKNAME> nickname_;
};

//int main() {
//    try {
//        boost::asio::io_service io_service;
//        tcp::resolver resolver(io_service);
//        tcp::resolver::query query("127.0.0.1", "8080");
//        tcp::resolver::iterator iterator = resolver.resolve(query);
//
//        std::array<char, MAX_NICKNAME> nickname = {"u"};
//
//        client_chat cli(nickname, io_service, iterator);
//
//        std::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
//
//        std::array<char, MAX_IP_PACK_SIZE> msg;
//
//        while (true) {
//            memset(msg.data(), '\0', msg.size());
//            // Функция, которая ожидает сообщение из окошка
//            // std::string message getMessage() {}
//
//
//            /*
//             * std::string getMessage(){
//             *  while(true){
//             *      if (message.got()){
//             *      break;
//             *      }
//             *  return message;
//             *  }
//             * }
//             * */
//
//            // Сообщение кастуется в string
//            std::string message = "Hello";
//            for(int i = 0; i < message.size(); i++) {
//                msg._M_elems[i] = message[i];
//            }
//
//            // Можно будет потом удалить эти 2 строки
//            std::string tmp;
//            std::cin >> tmp;
//
////            if (!std::cin.getline(msg.data(), MAX_IP_PACK_SIZE - PADDING - MAX_NICKNAME)) {
////                std::cin.clear(); //clean up error bit and try to finish reading
////            }
//
//            // Показываю написанное сообщение в консоль с именем и датой
//            cli.write(msg);
//        }
//
//        cli.close();
//        t.join();
//    } catch (std::exception& e) {
//        std::cerr << "Exception: " << e.what() << "\n";
//    }
//
//    return 0;
//}


