#include "widget.h"
#include "ui_widget.h"
#include "event.h"
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
#include <QStandardItemModel>
#include <QColor>
#include <QDateTime>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->stackedWidgetLogin->setCurrentIndex(0);
    mSocket = new QTcpSocket(this);
    ui->lineEdit_login->setInputMethodHints(Qt::ImhPreferNumbers);
    ui->turn_captcha_field->setInputMethodHints(Qt::ImhPreferNumbers);
    ui->turn_IP_field->setInputMethodHints(Qt::ImhFormattedNumbersOnly);
    ui->turn_port_field->setInputMethodHints(Qt::ImhPreferNumbers);
    ui->turn_IP_status->setText("Não conectado");
    ui->progressBar->hide();
    ui->progressBar->setRange(0,0);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->hist_labelPendente->setStyleSheet("QLabel { background-color : rgba(255,0,0,120)}");
    ui->hist_labelResolvido->setStyleSheet("QLabel { background-color : rgba(0,255,0,120)}");
    ui->buy_lineEditGoodThru->setInputMethodHints(Qt::ImhFormattedNumbersOnly);
    ui->buy_lineEditCardNumber->setInputMethodHints(Qt::ImhFormattedNumbersOnly);
    ui->buy_lineEditAmount->setInputMethodHints(Qt::ImhPreferNumbers);
    ui->buy_lineEditGoodThru->setInputMethodHints(Qt::ImhFormattedNumbersOnly);
    ui->buy_lineEditSecurityNumber->setInputMethodHints(Qt::ImhPreferNumbers);
    ui->buy_labelStatus->clear();
    ui->hist_tableWidgetEvents->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->hist_tableWidgetEvents->setFocusPolicy(Qt::NoFocus);
    ui->hist_tableWidgetEvents->setSelectionMode(QAbstractItemView::NoSelection);
    ui->info_tableWidgetLastEvent->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->info_tableWidgetLastEvent->setFocusPolicy(Qt::NoFocus);
    ui->info_tableWidgetLastEvent->setSelectionMode(QAbstractItemView::NoSelection);
}


Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButtonLogin_clicked()
{
    ui->pushButtonLogin->hide();
    ui->login_labelLoginStatus->clear();
    ui->login_labelLoginStatus->hide();
    ui->progressBar->show();
    Widget::userlogin = ui->lineEdit_login->text();
    Widget::password = ui->lineEdit_password->text();
    ui->login_labelLoginStatus->setStyleSheet("font-weight: normal; color: black");
    int login_status = Widget::authLogin();
    if (login_status==1){
        ui->pushButtonLogin->hide();
        ui->stackedWidgetNavigate->setCurrentIndex(0);
        ui->login_labelLoginStatus->setText("Carregando Dados do Usuário");
        Widget::requestUser();
        Widget::requestInsideRU();
        ui->stackedWidgetLogin->setCurrentIndex(1);
        ui->pushButtonLogin->show();
    }
    else{
        ui->pushButtonLogin->show();
        return;
    }
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
    QNetworkAccessManager networkManager;
    QWidget::connect(&networkManager, SIGNAL(finished(QNetworkReply*)),&eventLoop, SLOT(quit()));
    QNetworkReply *reply = networkManager.post(request, jsonData);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError)
    {
        QString strReply = (QString)reply->readAll();
        if (strReply == "Authentication error - User not found")
        {
            ui->progressBar->hide();
            ui->login_labelLoginStatus->show();
            ui->login_labelLoginStatus->clear();
            ui->login_labelLoginStatus->setText("Matrícula ou Senha Incorretos");
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
            ui->login_labelLoginStatus->setText("Erro Interno");
            return 0;
        }

        eventLoop.quit();
        delete reply;
    }
    else
    {
        ui->progressBar->hide();
        ui->login_labelLoginStatus->show();
        ui->login_labelLoginStatus->clear();
        ui->login_labelLoginStatus->setText("Sem Conexão");
        ui->login_labelLoginStatus->setStyleSheet("font-weight: bold; color: red");
        delete reply;
        return 0;
    }
}

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
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonArray json_array = jsonResponse.array();
        foreach (const QJsonValue & value, json_array)
        {
            QJsonObject objuser = value.toObject();
            Widget::tagNumber = objuser["tagNumber"].toString();
            Widget::creditsApp = objuser["cellphoneCredit"].toString();
            Widget::creditsCard = objuser["cardCredit"].toString();
            Widget::username = objuser["name"].toString();
            ui->info_labelName->setText(Widget::username);
            ui->info_labelLogin->setText(Widget::userlogin);
            ui->info_labelAPPCred->setText(Widget::creditsApp);
            ui->info_labelCardCred->setText(Widget::creditsCard);
            ui->hist_labelAPPCredit->setText(Widget::creditsApp);
            ui->hist_labelCardCredit->setText(Widget::creditsCard);
            ui->buy_labelAppCredits->setText(Widget::creditsApp);
            ui->buy_labelCardCredits->setText(Widget::creditsCard);
        }
        eventLoop.quit();
        delete reply;
    }
    else
    {
        delete reply;
    }
}

void Widget::requestLogs() {
    ui->hist_tableWidgetEvents->setRowCount(0);
    QEventLoop eventLoop;
    QUrl serviceUrl = QUrl("http://batata.dlinkddns.com/back-end/php/loadLogs.php");
    QNetworkRequest request(serviceUrl);
    QJsonObject json_user;
    json_user.insert("targetRegistry",Widget::userlogin);
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
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonArray json_array = jsonResponse.array();
        Widget::events_counter = -1;
        foreach (const QJsonValue & value, json_array)
        {
            Widget::events_counter++;
            ui->hist_tableWidgetEvents->insertRow(ui->hist_tableWidgetEvents->rowCount());
            QJsonObject objuser = value.toObject();
            Widget::eventos.setAll(objuser);
            ui->hist_tableWidgetEvents->setItem(Widget::events_counter,0,new QTableWidgetItem(Widget::eventos.getType()));
            ui->hist_tableWidgetEvents->setItem(Widget::events_counter,1,new QTableWidgetItem(Widget::eventos.getDiffApp()));
            ui->hist_tableWidgetEvents->setItem(Widget::events_counter,2,new QTableWidgetItem(Widget::eventos.getDiffCard()));
            ui->hist_tableWidgetEvents->setItem(Widget::events_counter,3,new QTableWidgetItem(Widget::eventos.getTime()));
            for (int i=0;i<=3;i++){
                ui->hist_tableWidgetEvents->item(Widget::events_counter,i)->setBackgroundColor(Widget::eventos.getResolved());
                ui->hist_tableWidgetEvents->item(Widget::events_counter,i)->setTextAlignment(Qt::AlignCenter);
            }
        }
        eventLoop.quit();
        delete reply;
    }
    else
    {
        delete reply;
    }
    ui->hist_labelLastUpdated->setText(QDateTime::currentDateTime().toString("hh:mm dd/MM"));
}


void Widget::requestInsideRU() {
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QWidget::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl( QString("http://batata.dlinkddns.com/back-end/php/loadAdminInfo.php") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called
    if (reply->error() == QNetworkReply::NoError) {
        QString strReply = (QString)reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonArray json_array = jsonResponse.array();
        QJsonObject obj = jsonResponse.object();
        QJsonValue insideRU = obj.value("insideRU");
        int insideRUnow = insideRU.toInt();
        ui->info_labelInsideRU->setText(QString::number(insideRUnow));
        eventLoop.quit();
        delete reply;
    }
    else {
        delete reply;
    }
    return;
}

void Widget::on_info_pushButtonRefresh_clicked()
{
    Widget::requestUser();
    Widget::requestInsideRU();
    Widget::requestLogs();
    Widget::updateLastEvent();
    return;
}

void Widget::on_hist_pushButtonRefresh_clicked()
{
    Widget::requestLogs();
    Widget::requestInsideRU();
    ui->hist_labelAPPCredit->setText(ui->info_labelAPPCred->text());
    ui->hist_labelCardCredit->setText(ui->info_labelCardCred->text());
    ui->hist_labelLastUpdated->setText(QDateTime::currentDateTime().toString("hh:mm dd/MM"));
    Widget::updateLastEvent();
}

void Widget::updateLastEvent(){
    ui->info_tableWidgetLastEvent->setRowCount(1);
    ui->info_tableWidgetLastEvent->setItem(0,0,new QTableWidgetItem(Widget::eventos.getType()));
    ui->info_tableWidgetLastEvent->setItem(0,1,new QTableWidgetItem(Widget::eventos.getDiffApp()));
    ui->info_tableWidgetLastEvent->setItem(0,2,new QTableWidgetItem(Widget::eventos.getDiffCard()));
    ui->info_tableWidgetLastEvent->setItem(0,3,new QTableWidgetItem(Widget::eventos.getTime()));
    for(int i=0;i<=3;i++){
        ui->info_tableWidgetLastEvent->item(0,i)->setBackgroundColor(Widget::eventos.getResolved());
        ui->info_tableWidgetLastEvent->item(0,i)->setTextAlignment(Qt::AlignCenter);
    }
}

void Widget::addCredits(){
    QEventLoop eventLoop;
    QUrl serviceUrl = QUrl("http://batata.dlinkddns.com/back-end/php/createCreditEvents.php");
    QNetworkRequest request(serviceUrl);
    QJsonObject json_credits;
    json_credits.insert("sourceRegistryNumber",Widget::userlogin);
    json_credits.insert("targetRegistryNumber",Widget::userlogin);
    json_credits.insert("diffCreditCellphone",Widget::buy_credits);
    json_credits.insert("diffCreditTag","0");
    QJsonDocument jsonDoc(json_credits);
    QByteArray jsonData= jsonDoc.toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    QNetworkAccessManager networkManager;
    QWidget::connect(&networkManager, SIGNAL(finished(QNetworkReply*)),&eventLoop, SLOT(quit()));
    QNetworkReply *reply = networkManager.post(request, jsonData);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError)
    {

        QString strReply = (QString)reply->readAll();
        if (strReply == "Update Successful! The credits will be updated as soon as the board syncs.")
        {
            ui->buy_labelStatus->setText("Sucesso!");
        }
        else
        {
            ui->buy_labelStatus->setText("Erro");
        }
        eventLoop.quit();
        delete reply;
    }
    else
    {
        ui->buy_labelStatus->setText("Sem Conexão");
        delete reply;
    }
}

void Widget::on_buy_lineEditAmount_editingFinished(){
    Widget::buy_credits = ui->buy_lineEditAmount->text();
    double total_price = Widget::buy_credits.toDouble()*1.5;
    ui->buy_labelPrice->setText("R$ "+QString::number(total_price,'f',2));
}

void Widget::on_buy_pushButtonBuy_clicked()
{
    ui->buy_labelStatus->setText("Processando");
    ui->buy_pushButtonBuy->hide();
    Widget::addCredits();
    delayms(1000);
    ui->buy_labelStatus->clear();
    ui->buy_pushButtonBuy->show();
}
