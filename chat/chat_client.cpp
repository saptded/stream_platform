#include "chat_client.h"


client::client(const std::array<char, MAX_NICKNAME>& nickname,
               boost::asio::io_service& io_service,
               boost::asio::ip::tcp::resolver::iterator endpoint_iterator) :
               io_service_(io_service), socket_(io_service) {

    strcpy(nickname_.data(), nickname.data());
    memset(read_msg_.data(), '\0', MAX_IP_PACK_SIZE);
    boost::asio::async_connect(socket_, endpoint_iterator, boost::bind(&client::onConnect, this, _1));
}

void client::write(const std::array<char, MAX_IP_PACK_SIZE>& msg) {
    io_service_.post(boost::bind(&client::writeImpl, this, msg));
}

void client::close() {
    io_service_.post(boost::bind(&client::closeImpl, this));
}

void client::onConnect(const boost::system::error_code& error) {
    if (!error) {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(nickname_, nickname_.size()),
                                 boost::bind(&client::readHandler, this, _1));
    }
}

void client::readHandler(const boost::system::error_code& error) {
    std::cout << read_msg_.data() << std::endl;
    if (!error) {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_, read_msg_.size()),
                                boost::bind(&client::readHandler, this, _1));
    } else {
        closeImpl();
    }
}

void client::writeImpl(std::array<char, MAX_IP_PACK_SIZE> msg) {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress) {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front(), write_msgs_.front().size()),
                                 boost::bind(&client::writeHandler, this, _1));
    }
}

void client::writeHandler(const boost::system::error_code& error) {
    if (!error) {
        write_msgs_.pop_front();
        if (!write_msgs_.empty()) {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(write_msgs_.front(), write_msgs_.front().size()),
                                     boost::bind(&client::writeHandler, this, _1));
        }
    } else {
        closeImpl();
    }
}

void client::closeImpl() {
    socket_.close();
}

//int main() {
//    try {
//        boost::asio::io_service io_service;
//        boost::asio::ip::tcp::resolver resolver(io_service);
//        boost::asio::ip::tcp::resolver::query query("127.0.0.1", "8080");
//        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
//
//        std::array<char, MAX_NICKNAME> nickname = {"u"};
//
//        client cli(nickname, io_service, iterator);
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
