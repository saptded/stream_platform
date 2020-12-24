#ifndef CLIENT_WINDOW_H
#define CLIENT_WINDOW_H

#include <QMainWindow>
#include <client.hpp>
#include "my_qthread.h"
#include <thread>
#include <mutex>


QT_BEGIN_NAMESPACE
namespace Ui { class client_window; }
QT_END_NAMESPACE

class client_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit client_window(sp::Client& client, QWidget *parent = nullptr);
    void send_button_clicked();
    void disconnect_button();
    void run_chat_server();
    void run_chat_client();

    ~client_window();

private:
  std::thread thread_chat_client;
    bool is_new_message = false;
    std::mutex mtx;
    std::string message;
    Ui::client_window *ui;
    sp::Client _client;
    AudioQThread *audio;
    QWidget *_parent;
};
#endif // CLIENT_WINDOW_H
