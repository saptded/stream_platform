#ifndef STREAMER_WINDOW_H
#define STREAMER_WINDOW_H

#include <QMainWindow>

#include "base_exception.h"

#include "my_qthread.h"

#include <streamer.hpp>
#include "client.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class streamer_window; }
QT_END_NAMESPACE

class streamer_window : public QMainWindow
{
    Q_OBJECT

public:
    streamer_window(sp::Streamer& streamer, QWidget *parent = nullptr);
    ~streamer_window();
    void run_server_chat();
    void disconnect_button();
    Ui::streamer_window *ui;
 public slots:
    void put_msg_into_window(const QString &msg);

 private:
    bool is_new_message = false;
    std::string message;
    std::thread thread_chat_server;
    QWidget *_parent;
    sp::Streamer _streamer;
    std::thread getting_usrs;
    std::thread streaming;
};


#endif // STREAMER_WINDOW_H
