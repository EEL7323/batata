#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class QTcpSocket;


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_connect_clicked();

    void on_IP_field_editingFinished();

    void on_port_field_editingFinished();

    void on_IP_field_returnPressed();

    void on_port_field_returnPressed();

    void on_captcha_field_returnPressed();

private:
    Ui::Widget *ui;
    QTcpSocket *mSocket;
};

#endif // WIDGET_H
