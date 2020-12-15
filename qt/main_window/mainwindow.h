#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

 public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

 private slots:
    void on_enter_name_line_textChanged(const QString &arg1);
    void on_enter_name_line_returnPressed();
    void on_confirm_button_clicked();

 private:
    Ui::MainWindow *ui;
    QString user_name;
    QLabel *error;
};
#endif  // MAINWINDOW_H
