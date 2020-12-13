#ifndef SECOND_WINDOW_H
#define SECOND_WINDOW_H

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>

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
    void settings_button();

 private:
    int role_id = 0;
    int settings_status = 0;
    QWidget *_parent;
    Ui::second_window *ui;
    QString _nick;
    QPushButton *back;
    QPushButton *extra_set;
    QLineEdit *data;
    QLineEdit *max_clients;
    QLineEdit *camera_index;
    QLabel *link_to_conf;
    QLabel *role;
    QLabel *m_cl;
    QLabel *c_i;
};
#endif  // SECOND_WINDOW_H
