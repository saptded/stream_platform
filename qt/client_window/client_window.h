#ifndef CLIENT_WINDOW_H
#define CLIENT_WINDOW_H

#include <QMainWindow>
#include <client.hpp>
#include "my_qthread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class client_window; }
QT_END_NAMESPACE

class client_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit client_window(sp::Client &client, QWidget *parent = nullptr);
    void disconnect_button();
    ~client_window();

private:
    Ui::client_window *ui;
    sp::Client _client;
    AudioQThread *audio;
    QWidget *_parent;
};
#endif // CLIENT_WINDOW_H
