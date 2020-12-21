#ifndef SECOND_WINDOW_H
#define SECOND_WINDOW_H

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <client.hpp>
#include <streamer.hpp>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class second_window;
}
QT_END_NAMESPACE

class second_window : public QMainWindow {
    Q_OBJECT

 public:
    second_window(QWidget *parent);
    void set_nick(QString &nick);
    ~second_window();

 private slots:
    void on_create_button_clicked();
    void on_join_button_clicked();
    void on_return_button_clicked();
    void back_button();
    void next_button();
    void settings_button();

 private:
    int role_id = 0;

    int settings_status = 0;

    int error_lay_status = 0;

    QWidget *_parent;
    Ui::second_window *ui;
    QPushButton *back;
    QPushButton *extra_set;
    QPushButton *next;
    QLineEdit *data;
    QSpinBox *max_clients;
    QSpinBox *camera_index;
    QLabel *link_to_conf;
    QLabel *role;
    QLabel *m_cl;
    QLabel *c_i;
    QLabel *connect_error;
    QLabel *invalid_link;
    QString entered_link;
    int max_clnts;
    int cam_index;

    sp::Client _client;
    sp::Streamer _streamer;
    QString _nick;
};
#endif  // SECOND_WINDOW_H
