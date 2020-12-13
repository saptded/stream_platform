#include "widget.h"
#include "./ui_widget.h"
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <utility>
#include "streamer.hpp"
#include <QTextBrowser>
#include "client.hpp"
#include <QLineEdit>
#include <QDebug>
#include <QtWidgets>
#include <QMainWindow>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_create_button_clicked()
{

    sp::Streamer host(_nick.toStdString());

    std::string str_link = host.create_link();
    QString link = QString::fromUtf8(str_link.c_str());

    QFont my_font("Ubuntu Mono", 18);
    my_font.setItalic(true);
    QFont my_font2("Ubuntu Mono", 42);
    my_font2.setItalic(true);

    QPushButton *back = new QPushButton("Back");
    back->setFont(my_font);
    connect(back, &QPushButton::clicked, this, &Widget::back_button);

    QLabel *role = new QLabel("HOST", this);
    role->setFont(my_font2);

    QLabel *link_to_conf = new QLabel("link to conference:", this);
    link_to_conf->setFont(my_font);

    QLineEdit *data = new QLineEdit(link);
    data->setReadOnly(true);
    data->setFont(my_font);

    data->setAlignment(Qt::AlignCenter);
    link_to_conf->setAlignment(Qt::AlignCenter);
    role->setAlignment(Qt::AlignCenter);

    this->hide();

//    ui->buttons_lay->itemAt(0)->widget()->hide();
//    ui->buttons_lay->itemAt(1)->widget()->hide();
//
//
//    ui->link_lay->addWidget(link_to_conf);
//    ui->link_lay->addWidget(data);
//    ui->buttons_lay->addWidget(role);
//    ui->accept_lay->addWidget(back);

}
void Widget::set_nick(QString &nick) {
    _nick = std::move(nick);
}

void Widget::on_join_button_clicked()
{
    sp::Client client(_nick.toStdString());

    QFont my_font("Ubuntu Mono", 18);
    my_font.setItalic(true);
    QFont my_font2("Ubuntu Mono", 42);
    my_font2.setItalic(true);

    QLabel *role = new QLabel("CLIENT", this);
    role->setFont(my_font2);

    QLabel *link_to_conf = new QLabel("enter link to conference:", this);
    link_to_conf->setFont(my_font);

    QLineEdit *data = new QLineEdit();
    data->setFont(my_font);
    QString got_link = data->text();
    std::string str = got_link.toStdString();

    data->setAlignment(Qt::AlignCenter);
    link_to_conf->setAlignment(Qt::AlignCenter);
    role->setAlignment(Qt::AlignCenter);



//    ui->buttons_lay->itemAt(0)->widget()->hide();
//    ui->buttons_lay->itemAt(1)->widget()->hide();
//
//    ui->link_lay->addWidget(link_to_conf);
//    ui->link_lay->addWidget(data);
//    ui->buttons_lay->addWidget(role);

}

void Widget::back_button() {
    Widget *w = new Widget();
    w->show();
    this->hide();
}