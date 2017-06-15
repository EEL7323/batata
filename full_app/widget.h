#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QMainWindow>
#include <QNetworkReply>
#include <QTextCodec>
#include <QtDebug>


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

    void on_pushButtonLogin_clicked();

    void on_pushButton_info_clicked();

    void on_pushButton_buy_clicked();

    void on_pushButton_hist_clicked();

    void on_pushButton_turn_clicked();

//Login
    int authLogin();

//TCP
    void on_turn_connect_clicked();

    void on_turn_IP_field_editingFinished();

    void on_turn_port_field_editingFinished();

    void on_turn_IP_field_returnPressed();

    void on_turn_port_field_returnPressed();

    void on_turn_captcha_field_returnPressed();

//HTTP
    void on_info_pushButtonRefresh_clicked();

    void requestUser();

    void requestLogs();

private:
    QString userlogin="13200964";
    QString tagNumber = "0";
    Ui::Widget *ui;
    QTcpSocket *mSocket;
    QNetworkAccessManager *nam;
};

#endif // WIDGET_H
