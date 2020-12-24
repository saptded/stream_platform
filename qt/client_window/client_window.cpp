#include "client_window.h"

#include <utility>
#include "./ui_client_window.h"
#include "video_receiver.h"
#include "protocol.h"
#include <deque>
#include <array>
#include <thread>
#include <iostream>
#include <cstring>
#include <QDebug>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "protocol.h"
#include "chat_client.h"
#include <mutex>

client_window::client_window(sp::Client& client, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::client_window), _client(client)
{
    ui->setupUi(this);


    connect(ui->disconnect_button, &QPushButton::clicked, this, &client_window::disconnect_button);
    connect(ui->send_button, &QPushButton::clicked, this, &client_window::send_button_clicked);

    _parent = parent;

    ui->video_label = new VideoReceiver{_client, this};
    ui->video_label->setGeometry(100, 50, 1280, 720);

    audio = new AudioQThread(_client);

    audio->start();

    thread_chat_client = std::thread(&client_window::run_chat_client,this);

}

void client_window::run_chat_client() {
        try {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(_client.server_ip, std::to_string(8200));
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);


        std::array<char, MAX_NICKNAME> nickname = {""};

        for(int i = 0; i < _client.nickname.size() && i < MAX_NICKNAME; i++) {
            nickname._M_elems[i] = _client.nickname[i];
        }


        client cli(nickname, io_service, iterator);

        std::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

        std::array<char, MAX_IP_PACK_SIZE> msg;

        while (true) {
            sleep(0.1);
            memset(msg.data(), '\0', msg.size());
            // Функция, которая ожидает сообщение из окошка
            // std::string message getMessage() {}


            /*
             * std::string getMessage(){
             *  while(true){
             *      if (message.got()){
             *      break;
             *      }
             *  return message;
             *  }
             * }
             * */

            // Сообщение кастуется в string

            if (is_new_message) {
//                std::string message = "Hello";
                mtx.lock();
                for(int i = 0; i < message.size(); i++) {
                    msg._M_elems[i] = message[i];
                }
                mtx.unlock();

                // Можно будет потом удалить эти 2 строки
//                std::string tmp;
//                std::cin >> tmp;

//            if (!std::cin.getline(msg.data(), MAX_IP_PACK_SIZE - PADDING - MAX_NICKNAME)) {
//                std::cin.clear(); //clean up error bit and try to finish reading
//            }

                // Показываю написанное сообщение в консоль с именем и датой
                cli.write(msg);

                mtx.lock();
                message.clear();
                is_new_message = false;
                mtx.unlock();
            }

        }

        cli.close();
        t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

}

client_window::~client_window()
{
    thread_chat_client.join();
    audio->wait();

    delete ui;
}

void client_window::disconnect_button() {
    audio->stop_gst_audio_loop();
    audio->exit(0);
    _parent->show();
    delete this;


}

void client_window::send_button_clicked() {

    QString QStr = ui->enter_msg_field->toPlainText();
    mtx.lock();
    message = QStr.toStdString();
    qDebug() << QStr;
    is_new_message = true;
    mtx.unlock();

}
