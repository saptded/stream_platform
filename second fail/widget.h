#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void set_nick(QString &nick);
    ~Widget();

private slots:
    void on_create_button_clicked();
    void on_join_button_clicked();
    void back_button();

private:
    Ui::Widget *ui;
    Ui::Widget *ui1;
    QString _nick;
};
#endif // WIDGET_H
