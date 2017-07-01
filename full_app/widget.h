/*
 * Widget class, where the entire app runs on
 */

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
#include <QJsonObject>


namespace Ui {
class Widget;
}

// TCP socket
class QTcpSocket;

// 
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

	// Login button push method
    void on_pushButtonLogin_clicked();
	
	//		Navigation related methods (in the app)
	// Info button push method (to navigate)
    void on_pushButton_info_clicked();
	// Buy button push method (to navigate)
    void on_pushButton_buy_clicked();
	// Hist button push method (to navigate)
    void on_pushButton_hist_clicked();
	// Turn button push method (to navigate)
    void on_pushButton_turn_clicked();

	// Login authentication method (through HTTP)
    int authLogin();

	// 		Turnstile related methods (involving TCP):
	// Method to start the connection when the connect button is pressed
    void on_turn_connect_clicked();
	// Method to refresh the UI to show the IP was recognized (Debug only)
    void on_turn_IP_field_editingFinished();
	// Method to refresh the UI to show the Port was recognized (Debug only)
    void on_turn_port_field_editingFinished();
	// Method to refresh the UI to show the IP was recognized (Debug only)
    void on_turn_IP_field_returnPressed();
	// Method to refresh the UI to show the Port was recognized (Debug only)
    void on_turn_port_field_returnPressed();
	// Method to start the connection when it's pressed "return" on the captcha-field
    void on_turn_captcha_field_returnPressed();

	// 		Server related methods (involving HTTP):
	// Method to handle the Refresh button click (in the "Info" page).
    void on_info_pushButtonRefresh_clicked();
	// Method to request the user's info
    void requestUser();
	// Method to request the user's logs
    void requestLogs();
	// Method to request the number of people currently inside the RU
    void requestInsideRU();
	// Method to handle the Refresh button click (in the "Hist" page)
    void on_hist_pushButtonRefresh_clicked();
	// Method to update the last event on the "Info" page
    void updateLastEvent();

	// 		Credits-purchase related methods:
	// Method to add credits to the account (HTTP)
    void addCredits();
	// Method to refresh the UI to show the price (in R$)
    void on_buy_lineEditAmount_editingFinished();
	// Method to handle the Buy button click
    void on_buy_pushButtonBuy_clicked();

private:
	// User atributes:
    QString userlogin="";
    QString password = "";
    QString tagNumber = "";
    QString creditsApp = "";
    QString creditsCard = "";
    QString username = "";
    
	// Amount of credits to buy:
	QString buy_credits = "";
	
	// Interface:
    Ui::Widget *ui;
	
	// TCP Socket:
    QTcpSocket *mSocket;
	
	// HTTP Network Manager:
    QNetworkAccessManager *nam;
	
	// Event (used to keep the last event):
    Event eventos;
	
	// Event counter, to keep track of the number of events:
    int events_counter;
};



#endif // WIDGET_H
