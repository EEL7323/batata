#include "widget.h"
#include "ui_widget.h"
#include <QTcpSocket>
#include <QTextStream>
#include <QTime>
#include <QThread>
#include <QHostAddress>
#include <QJsonObject>
#include <QtNetwork>
#include <QUrl>
#include <QJsonDocument>
#include <QHeaderView>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->stackedWidgetLogin->setCurrentIndex(0);
    mSocket = new QTcpSocket(this);
    ui->turn_captcha_field->setInputMethodHints(Qt::ImhPreferNumbers);
    ui->turn_IP_field->setInputMethodHints(Qt::ImhFormattedNumbersOnly);
    ui->turn_port_field->setInputMethodHints(Qt::ImhPreferNumbers);
    ui->turn_IP_status->setText("Não conectado");

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButtonLogin_clicked()
{
    ui->stackedWidgetLogin->setCurrentIndex(Widget::authLogin());
    ui->stackedWidgetNavigate->setCurrentIndex(0);
}

void Widget::on_pushButton_info_clicked()
{
    ui->stackedWidgetNavigate->setCurrentIndex(0);
}

void Widget::on_pushButton_buy_clicked()
{
    ui->stackedWidgetNavigate->setCurrentIndex(1);
}

void Widget::on_pushButton_hist_clicked()
{
    ui->stackedWidgetNavigate->setCurrentIndex(2);
}

void Widget::on_pushButton_turn_clicked()
{
    ui->stackedWidgetNavigate->setCurrentIndex(3);
}


//LOGIN


int Widget::authLogin(){
/*
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QWidget::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req_post( QUrl( QString("http://177.42.55.96/back-end/php/login.php") ) );
    //QHttpMultiPart
    QString post_str = "[{'registryNumber':"+ui->lineEdit_login->text()+",'password':"+ui->lineEdit_password->text()+"}]";
    QNetworkReply *reply = mgr.post(req_post,post_str);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    //updateCredits.php
    //"sourceRegistryNumber":"user"
    //"targetRegistryNumber":"user"
    //"diffCreditCellphone":"10"
    //"diffCreditTag":"0"

    if (reply->error() == QNetworkReply::NoError)
    {

        QString strReply = (QString)reply->readAll();

        //parse json
        if (strReply == "[{'y'}]")
        {
            return 1;
        }
        else
        {
            return 0;
        }
        eventLoop.quit();
        //ui->statusLabel->setText("Success!");
        delete reply;
    }
    else
    {
        //failure
        //qDebug() << "Failure" <<reply->errorString();
        //ui->statusLabel->setText("Failure :(");
        delete reply;
        return 0;
    }
    */
    return 1;
}


//TURNSTILE


void delayms( int millisecondsToWait ){
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}


void Widget::on_turn_connect_clicked()
{

    if ((ui->turn_IP_field->text()=="")||(ui->turn_port_field->text()=="")){
        mSocket->connectToHost("192.168.25.6", 80); //Local Host's Address
        ui->turn_IP_status->setText("Usando 192.168.25.6:80");
    }
    else{
        //QHostAddress ip_address = QHostAddress(ui->IP_field->text());
        mSocket->connectToHost(ui->turn_IP_field->text(), ui->turn_port_field->text().toInt()); //Local Host's Address
        ui->turn_IP_status->setText("Usando "+ui->turn_IP_field->text()+":"+ui->turn_port_field->text());
    }

    mSocket->waitForConnected(500);
    if ((mSocket == nullptr)||(mSocket->state() != QTcpSocket::ConnectedState)){
        ui->turn_IP_status->setText("IP ou porta inválidos");
        return;
    }
    else{
        ui->turn_IP_status->setText("Conectou");
    }
    QTextStream captcha(mSocket);
    captcha << Widget::tagNumber+ui->turn_captcha_field->text();
    mSocket->flush();

    connect(mSocket, &QTcpSocket::readyRead, [&]()
    {
        QTextStream T(mSocket);
        auto text = T.readAll();

        if (text == "y")
        {
            ui->turn_status->setText("Acesso Liberado");
            delayms(100);
            QThread::msleep(5000);
           }
        else if (text == "n")
        {
            ui->turn_status->setText("Captcha Incorreto");
            delayms(100);
            QThread::msleep(1000);

        }

        delayms(500);
        ui->turn_status->setText("Espere Sua Vez");
        ui->turn_IP_status->setText("Não conectado");
        mSocket->disconnectFromHost();
        mSocket->close();
        return;

    });

}

void Widget::on_turn_IP_field_editingFinished()
{
    ui->turn_IP_status->setText("Usando "+ui->turn_IP_field->text()+":"+ui->turn_port_field->text());
}

void Widget::on_turn_port_field_editingFinished()
{
    ui->turn_IP_status->setText("Usando "+ui->turn_IP_field->text()+":"+ui->turn_port_field->text());
}

void Widget::on_turn_IP_field_returnPressed()
{
    ui->turn_IP_status->setText("Usando "+ui->turn_IP_field->text()+":"+ui->turn_port_field->text());
}

void Widget::on_turn_port_field_returnPressed()
{
    ui->turn_IP_status->setText("Usando "+ui->turn_IP_field->text()+":"+ui->turn_port_field->text());
}

void Widget::on_turn_captcha_field_returnPressed()
{
    Widget::on_turn_connect_clicked();
}


//INFO


void Widget::requestUser() {

    // create custom temporary event loop on stack
    QEventLoop eventLoop;


    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QWidget::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl( QString("http://177.42.55.96/back-end/php/loadStudents.php") ) );

    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {

        QString strReply = (QString)reply->readAll();

        //parse json

        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonArray json_array = jsonResponse.array();

        foreach (const QJsonValue & value, json_array)
        {
            QJsonObject obj = value.toObject();

            if (obj["registryNumber"].toString() == Widget::userlogin)
            {
                ui->info_labelName->setText(obj["name"].toString());
                ui->info_labelLogin->setText(Widget::userlogin);
                ui->info_labelAPPCred->setText(obj["cellphoneCredit"].toString());
                ui->info_labelCardCred->setText(obj["cardCredit"].toString());
                Widget::tagNumber = obj["tagNumber"].toString();
            }

        }
        eventLoop.quit();
        //ui->statusLabel->setText("Success!");
        delete reply;
    }
    else {
        //failure
        //qDebug() << "Failure" <<reply->errorString();
        //ui->statusLabel->setText("Failure :(");
        delete reply;
    }
}

void Widget::requestLogs() {

    // create custom temporary event loop on stack
    QEventLoop eventLoop;


    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QWidget::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req( QUrl( QString("http://192.168.43.117:8081/server/back-end/php/loadEvents.php") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {

        QString strReply = (QString)reply->readAll();

        //parse json

        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonArray json_array = jsonResponse.array();

        foreach (const QJsonValue & value, json_array)
        {
            QJsonObject obj = value.toObject();

            if (obj["registryNumber"].toString() == userlogin)
            {
                ui->info_tableWidgetLastEvent->setItem(1,1,new QTableWidgetItem(obj["type"].toString()));
                ui->info_tableWidgetLastEvent->setItem(1,2,new QTableWidgetItem(obj["diffCredCellphone"].toString()));
                ui->info_tableWidgetLastEvent->setItem(1,3,new QTableWidgetItem(obj["diffCredTag"].toString()));
                ui->info_tableWidgetLastEvent->setItem(1,4,new QTableWidgetItem(obj["time"].toString()));
            }

        }
        eventLoop.quit();
        //ui->statusLabel->setText("Success!");
        delete reply;
    }
    else {
        //ui->statusLabel->setText("Failure :(");
        delete reply;
    }

}

void Widget::on_info_pushButtonRefresh_clicked()
{
    //ui->statusLabel->clear();
    Widget::requestUser();
    //Widget::requestLogs();
    return;
}


