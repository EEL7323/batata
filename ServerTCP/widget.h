#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class QTcpServer;
class QTcpSocket;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_send_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *mServer;
    QTcpSocket *mSocket;
};

#endif // WIDGET_H
