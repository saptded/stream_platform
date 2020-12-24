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
    void disconnect_button();

private:
    Ui::streamer_window *ui;
    QWidget *_parent;
    sp::Streamer _streamer;
    std::thread getting_usrs;
    std::thread streaming;
};
#endif // STREAMER_WINDOW_H
