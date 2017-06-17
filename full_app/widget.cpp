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
#include <QtDebug>


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
    ui->progressBar->hide();
    ui->progressBar->setRange(0,0);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButtonLogin_clicked()
{
    ui->login_labelLoginStatus->clear();
    ui->login_labelLoginStatus->hide();
    ui->progressBar->show();
    Widget::userlogin = ui->lineEdit_login->text();
    Widget::password = ui->lineEdit_password->text();
    ui->login_labelLoginStatus->setStyleSheet("font-weight: normal; color: black");
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
    QUrl serviceUrl = QUrl("http://batata.dlinkddns.com/back-end/php/login.php");
    QNetworkRequest request(serviceUrl);
    QJsonObject json_login;
    json_login.insert("registry",Widget::userlogin);
    json_login.insert("password",Widget::password);
    json_login.insert("fromApp",1);
    QJsonDocument jsonDoc(json_login);
    QByteArray jsonData= jsonDoc.toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    //request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(jsonData.size()));
    QNetworkAccessManager networkManager;
    QWidget::connect(&networkManager, SIGNAL(finished(QNetworkReply*)),&eventLoop, SLOT(quit()));
    QNetworkReply *reply = networkManager.post(request, jsonData);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {

        QString strReply = (QString)reply->readAll();

        qDebug() << "\n\n"<<strReply<<"\n";

        if (strReply == "Authentication error - User not found")
        {
            ui->progressBar->hide();
            ui->login_labelLoginStatus->show();
            ui->login_labelLoginStatus->clear();
            ui->login_labelLoginStatus->setText("Matrícula ou Senha incorretos");
            return 0;
        }
        else if (strReply == "Login Successful")
        {
            ui->progressBar->hide();
            ui->login_labelLoginStatus->show();
            ui->login_labelLoginStatus->clear();
            return 1;
        }
        else
        {
            ui->progressBar->hide();
            ui->login_labelLoginStatus->show();
            ui->login_labelLoginStatus->clear();
            ui->login_labelLoginStatus->setText("Matrícula ou Senha Incorretos");
            return 0;
        }

        eventLoop.quit();
        //ui->statusLabel->setText("Success!");
        delete reply;
    }
    else
    {
        //failure
        qDebug() << "Failure";// <<reply->errorString();
        ui->progressBar->hide();
        ui->login_labelLoginStatus->show();
        ui->login_labelLoginStatus->clear();
        ui->login_labelLoginStatus->setText("Sem Conexão");
        ui->login_labelLoginStatus->setStyleSheet("font-weight: bold; color: red");
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
        else if (text == "ooc")
        {
            ui->turn_status->setText("Saldo Insuficiente");
            delayms(100);
            QThread::msleep(1000);
        }
        else if (text == "unr")
        {
            ui->turn_status->setText("Usuário Não Cadastrado");
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


    QEventLoop eventLoop;
    QUrl serviceUrl = QUrl("http://batata.dlinkddns.com/back-end/php/loadUsers.php");
    QNetworkRequest request(serviceUrl);
    QJsonObject json_user;
    json_user.insert("registryNumber",Widget::userlogin);
    QJsonDocument jsonDoc(json_user);
    QByteArray jsonData= jsonDoc.toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    QNetworkAccessManager networkManager;
    QWidget::connect(&networkManager, SIGNAL(finished(QNetworkReply*)),&eventLoop, SLOT(quit()));
    QNetworkReply *reply = networkManager.post(request, jsonData);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {

        QString strReply = (QString)reply->readAll();

        qDebug() << "\n\n"<<strReply<<"\n";
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonArray json_array = jsonResponse.array();
        qDebug() << json_array;
        QJsonObject obj = jsonResponse.object();
        /*
        ui->info_labelName->setText(obj["name"].toString());
        ui->info_labelLogin->setText(Widget::userlogin);
        ui->info_labelAPPCred->setText(obj["cellphoneCredit"].toString());
        ui->info_labelCardCred->setText(obj["cardCredit"].toString());
        Widget::tagNumber = obj["tagNumber"].toString();
        */
        eventLoop.quit();
        //ui->statusLabel->setText("Success!");
        delete reply;
    }
    else
    {
        //failure
        qDebug() << "Failure";// <<reply->errorString();
        delete reply;
    }



/*

    // create custom temporary event loop on stack
    QEventLoop eventLoop;


    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QWidget::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl( QString("http://batata.dlinkddns.com/back-end/php/loadUsers.php") ) );

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
    */
}

void Widget::requestLogs() {

    // create custom temporary event loop on stack
    QEventLoop eventLoop;


    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QWidget::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req( QUrl( QString("http://batata.dlinkddns.com/back-end/php/loadLogs.php") ) );
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
    return;

}


void Widget::requestInsideRU() {

    // create custom temporary event loop on stack
    QEventLoop eventLoop;
    qDebug() << "\n\nin event\n\n";
    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QWidget::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req( QUrl( QString("http://batata.dlinkddns.com/back-end/php/loadAdminInfo.php") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called
    qDebug() << "\n\nrunning\n\n";
    if (reply->error() == QNetworkReply::NoError) {

        QString strReply = (QString)reply->readAll();

        //parse json

        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonArray json_array = jsonResponse.array();
        qDebug() << json_array;
        QJsonObject obj = jsonResponse.object();
        QJsonValue insideRU = obj.value("insideRU");
        int insideRUnow = insideRU.toInt();
        qDebug() <<"\n\n"<<QString::number(insideRUnow)<<"\n\n";
        ui->info_labelInsideRU->setText(QString::number(insideRUnow));
        /*
        foreach (const QJsonValue & value, json_array)
        {
            QJsonObject obj = value.toObject();
            qDebug() << "\n\n"+obj.value("insideRU").toString()+"\n\n";
            ui->info_labelInsideRU->setText(obj.value("lastLunch").toString());

        }
        */
        qDebug() << "\n\nend\n\n";
        eventLoop.quit();
        //ui->statusLabel->setText("Success!");
        delete reply;
    }
    else {
        qDebug() << "\n\nerror\n\n";
        //ui->statusLabel->setText("Failure :(");
        delete reply;
    }
    return;

}


void Widget::on_info_pushButtonRefresh_clicked()
{
    qDebug() << "\n\ntrying to request\n\n";
    Widget::requestUser();
    Widget::requestInsideRU();
    //Widget::requestLogs();
    return;
}


