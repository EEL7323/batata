#ifndef WIDGET_H
#define WIDGET_H

#include "event.h"
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

    void requestInsideRU();

    void on_hist_pushButtonRefresh_clicked();

    void updateLastEvent();

//BUY
    void addCredits();

    void on_buy_lineEditAmount_editingFinished();

    void on_buy_pushButtonBuy_clicked();

private:
    QString userlogin="";
    QString password = "";
    QString tagNumber = "";
    QString creditsApp = "";
    QString creditsCard = "";
    QString username = "";
    QString buy_credits = "";
    Ui::Widget *ui;
    QTcpSocket *mSocket;
    QNetworkAccessManager *nam;
    Event eventos;
    int events_counter;
};



#endif // WIDGET_H
