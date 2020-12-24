#include "mainwindow.h"

#include "./ui_mainwindow.h"
#include "second_window.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) { ui->setupUi(this); }

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_enter_name_line_textChanged(const QString &arg1) {
    user_name = arg1;

    std::string def = user_name.toStdString();
    std::string str;

    for (int i = 0; i < def.size(); i++) {
        if (def[i] != ' ') {
            for (i; i < def.size(); i++) {
                if (def[i] == ' ') {
                    str += '_';
                } else {
                    str += def[i];
                }
            }
        }
    }

    for (int j = str.size() - 1; j != 0; j--) {
        if (str[j] == '_') {
            str.erase(j, 1);
        } else {
            break;
        }
    }

    if (!str.empty()) {
        user_name = QString::fromStdString(str);
    } else {
        ui->enter_name_line->clear();
    }
}

void MainWindow::on_confirm_button_clicked() {
    if (user_name.isEmpty() && ui->error_lay->isEmpty()) {
        error = new QLabel("the input field is empty");
        error->setStyleSheet("QLabel {color: rgb(238, 238, 236)}");
        ui->error_lay->addWidget(error);
    }
    if (!user_name.isEmpty()) {
        if (!ui->error_lay->isEmpty()) {
            delete error;
        }
        auto *w = new second_window(this);
        w->set_nick(user_name);
        w->show();
        this->hide();
        ui->enter_name_line->clear();
    }
}

void MainWindow::on_enter_name_line_returnPressed() {
    if (user_name.isEmpty() && ui->error_lay->isEmpty()) {
        error = new QLabel("the input field is empty");
        error->setStyleSheet("QLabel {color: rgb(238, 238, 236)}");
        ui->error_lay->addWidget(error);
    }
    if (!user_name.isEmpty()) {
        if (!ui->error_lay->isEmpty()) {
            delete error;
        }
        auto *w = new second_window(this);
        w->set_nick(user_name);
        w->show();
        this->hide();
        ui->enter_name_line->clear();
    }
}

