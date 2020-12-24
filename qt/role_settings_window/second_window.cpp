#include "second_window.h"

#include "./ui_second_window.h"

#include <QLineEdit>
#include <QWidget>
#include <base_exception.h>
#include <client.hpp>
#include <client_window.h>
#include <streamer.hpp>
#include <streamer_window.h>

second_window::second_window(QWidget *parent) : QMainWindow(parent), ui(new Ui::second_window) {
    _parent = parent;
    ui->setupUi(this);
}

second_window::~second_window() { delete ui; }

void second_window::on_create_button_clicked() {
    role_id = 1;

    if (port_status == 1) {
        //        delete data;
    }

    ui->data_widget->show();

    if (port_status == 0) {
        _streamer = sp::Streamer(_nick.toStdString());
    }

    std::string str_link = _streamer.create_link();
    QString link = QString::fromUtf8(str_link.c_str());

    QFont my_font("Ubuntu Mono", 14);
    my_font.setItalic(true);
    QFont my_font2("Ubuntu Mono", 42);
    my_font2.setItalic(true);

    back = new QPushButton("Back");
    connect(back, &QPushButton::clicked, this, &second_window::back_button);
    back->setFont(my_font);
    back->setStyleSheet("QPushButton {color: rgb(238, 238, 236)}");
    ui->main_buttons_lay->addWidget(back);

    next = new QPushButton("Next");
    connect(next, &QPushButton::clicked, this, &second_window::next_button);
    next->setFont(my_font);
    next->setStyleSheet("QPushButton {color: rgb(238, 238, 236)}");
    ui->main_buttons_lay->addWidget(next);


    role = new QLabel("HOST", this);
    role->setFont(my_font2);
    role->setStyleSheet("QLabel {color: rgb(238, 238, 236)}");
    ui->label_lay->addWidget(role);

    link_to_conf = new QLabel("link to conference:", this);
    link_to_conf->setFont(my_font);
    link_to_conf->setStyleSheet("QLabel {color: rgb(238, 238, 236)}");
    ui->link_lay->addWidget(link_to_conf);

    data = new QLineEdit(link);
    data->setReadOnly(true);
    data->setFont(my_font);
    data->setStyleSheet("QLineEdit {color: rgb(238, 238, 236)}");
    ui->link_lay->addWidget(data);

    extra_set = new QPushButton("settings");
    extra_set->setStyleSheet("QPushButton {color: rgb(238, 238, 236)}");
    ui->show_extra_settings_lay->addWidget(extra_set);
    connect(extra_set, &QPushButton::clicked, this, &second_window::settings_button);

    data->setAlignment(Qt::AlignCenter);
    link_to_conf->setAlignment(Qt::AlignCenter);
    role->setAlignment(Qt::AlignCenter);

    ui->role_buttons_widget->hide();

}

void second_window::on_join_button_clicked() {
    role_id = 2;

    ui->data_widget->show();
    _client = sp::Client(_nick.toStdString());

    QFont my_font("Ubuntu Mono", 14);
    my_font.setItalic(true);
    QFont my_font2("Ubuntu Mono", 42);
    my_font2.setItalic(true);

    back = new QPushButton("Back");
    connect(back, &QPushButton::clicked, this, &second_window::back_button);
    back->setFont(my_font);
    back->setStyleSheet("QPushButton {color: rgb(238, 238, 236)}");
    ui->main_buttons_lay->addWidget(back);

    next = new QPushButton("Next");
    connect(next, &QPushButton::clicked, this, &second_window::next_button);
    next->setFont(my_font);
    next->setStyleSheet("QPushButton {color: rgb(238, 238, 236)}");
    ui->main_buttons_lay->addWidget(next);

    role = new QLabel("CLIENT", this);
    role->setFont(my_font2);
    role->setStyleSheet("QLabel {color: rgb(238, 238, 236)}");
    role->setAlignment(Qt::AlignCenter);
    ui->label_lay->addWidget(role);

    link_to_conf = new QLabel("enter link to conference:", this);
    link_to_conf->setFont(my_font);
    link_to_conf->setStyleSheet("QLabel {color: rgb(238, 238, 236)}");
    link_to_conf->setAlignment(Qt::AlignCenter);
    ui->link_lay->addWidget(link_to_conf);

    data = new QLineEdit();
    data->setFont(my_font);
    entered_link = data->text();
    data->setAlignment(Qt::AlignCenter);
    data->setStyleSheet("QLineEdit {color: rgb(238, 238, 236)}");
    ui->link_lay->addWidget(data);

    ui->role_buttons_widget->hide();
}

void second_window::set_nick(QString &nick) { _nick = std::move(nick); }

void second_window::back_button() {
    ui->role_buttons_widget->show();
    ui->data_widget->hide();
    delete back;
    delete next;
    delete data;
    delete link_to_conf;
    delete role;
    if (role_id == 1) {
        delete extra_set;
        if (settings_status == 1 || settings_status == 2) {
            delete m_cl;
            delete max_clients;
            delete c_i;
            delete camera_index;
            settings_status = 0;
        }
    }
    if (role_id == 2) {
        if (error_lay_status == 2) {
            delete invalid_link;
        }
        if (error_lay_status == 3) {
            delete connect_error;
        }
    }
    error_lay_status = 0;
    role_id = 0;
}

void second_window::on_return_button_clicked() {
    _parent->show();
    delete this;
}

void second_window::settings_button() {
    if (settings_status == 0) {
        QFont my_font("Ubuntu Mono", 14);
        my_font.setItalic(true);

        m_cl = new QLabel("max clients:  ", this);
        m_cl->setFont(my_font);
        m_cl->setStyleSheet("QLabel {color: rgb(238, 238, 236)}");
        ui->max_clients_lay->addWidget(m_cl);

        max_clients = new QSpinBox();
        max_clients->setFont(my_font);
        max_clients->setStyleSheet("QSpinBox {color: rgb(238, 238, 236)}");
        ui->max_clients_lay->addWidget(max_clients);

        c_i = new QLabel("camera index: ", this);
        c_i->setFont(my_font);
        c_i->setStyleSheet("QLabel {color: rgb(238, 238, 236)}");
        ui->camera_index_lay->addWidget(c_i);

        camera_index = new QSpinBox();
        camera_index->setFont(my_font);
        camera_index->setStyleSheet("QSpinBox {color: rgb(238, 238, 236)}");
        ui->camera_index_lay->addWidget(camera_index);

        settings_status = 1;
    } else if (settings_status == 1) {
        ui->settings_widget->hide();
        settings_status = 2;
    } else if (settings_status == 2) {
        ui->settings_widget->show();
        settings_status = 1;
    }
}

void second_window::next_button() {
    QFont my_font("Ubuntu Mono", 14);
    my_font.setItalic(true);

    if (role_id == 2) {
        try {
            entered_link = data->text();

            std::string tmp = entered_link.toStdString();

            if (tmp[0] == '.') {
                _client.get_link(tmp);
                QMainWindow *client_win = new client_window(_client, this);
                client_win->show();
                this->hide();
            } else {
                if (error_lay_status == 3) {
                    delete connect_error;
                    invalid_link = new QLabel("invalid link");
                    invalid_link->setFont(my_font);
                    invalid_link->setAlignment(Qt::AlignRight);
                    ui->errors_lay->addWidget(invalid_link);
                    error_lay_status = 2;
                }
                if (error_lay_status == 0) {
                    invalid_link = new QLabel("invalid link");
                    invalid_link->setFont(my_font);
                    invalid_link->setAlignment(Qt::AlignRight);
                    ui->errors_lay->addWidget(invalid_link);
                    error_lay_status = 2;
                }
            }

        } catch (BaseException &err) {
            if (error_lay_status == 2) {
                delete invalid_link;
                connect_error = new QLabel("connect error");
                connect_error->setFont(my_font);
                connect_error->setAlignment(Qt::AlignRight);
                ui->errors_lay->addWidget(connect_error);
                error_lay_status = 3;
            }
            if (error_lay_status == 0) {
                connect_error = new QLabel("connect error");
                connect_error->setFont(my_font);
                connect_error->setAlignment(Qt::AlignRight);
                ui->errors_lay->addWidget(connect_error);
                error_lay_status = 3;
            }
        }
    }
    if (role_id == 1) {
        try {
            if (settings_status == 1 || settings_status == 2) {
                max_clnts = max_clients->value() + 1;
                _streamer.set_max_client_amount(max_clnts);

                cam_index = camera_index->value();
                _streamer.set_cam_index(cam_index);
            }
            if (port_status == 1) {}

            port_status = 1;

            streamer_win = new streamer_window(_streamer, this);
            streamer_win->show();
            this->hide();

            _streamer.change_port();
            delete data;
            std::string str_link = _streamer.create_link();
            QString link = QString::fromUtf8(str_link.c_str());
            data = new QLineEdit(link);
            data->setReadOnly(true);
            data->setFont(my_font);
            data->setAlignment(Qt::AlignCenter);
            ui->link_lay->addWidget(data);
        } catch (BaseException &err) {}
    }
}