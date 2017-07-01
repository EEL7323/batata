/*
 * APP methods' code
 * User Interface (ui) updates occur in many places of the code,
 * thus they won't be detailedly explained along the codelines.
 */
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
	// UI startup configuration
    ui->setupUi(this);
    ui->stackedWidgetLogin->setCurrentIndex(0);
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
	
	// New TCP Socket
	mSocket = new QTcpSocket(this);
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
	// Gets the login and password from their respective fields in the Login page
    Widget::userlogin = ui->lineEdit_login->text();
    Widget::password = ui->lineEdit_password->text();
    ui->login_labelLoginStatus->setStyleSheet("font-weight: normal; color: black");
	// Runs authLogin() function and returns the result (1 = Success, 0 = Failure)
    int login_status = Widget::authLogin();
    if (login_status==1){
        ui->pushButtonLogin->hide();
        ui->stackedWidgetNavigate->setCurrentIndex(0);
        ui->login_labelLoginStatus->setText("Carregando Dados do Usuário");
		// If the login is successful, it is requested the user info and the
		// number of people inside the RU from the server (using requestUser
		// and requestInsideRU).
        Widget::requestUser();
        Widget::requestInsideRU();
        ui->stackedWidgetLogin->setCurrentIndex(1);
        ui->pushButtonLogin->show();
    }
    else{
		// Else, it fails and doesn't allow the user to access the rest of
		// the app.
        ui->pushButtonLogin->show();
        return;
    }
}

void Widget::on_pushButton_info_clicked()
{
	// Changes the Page to INFO when the button Info is clicked
    ui->stackedWidgetNavigate->setCurrentIndex(0);
}

void Widget::on_pushButton_buy_clicked()
{
	// Changes the Page to BUY when the button Buy is clicked
    ui->stackedWidgetNavigate->setCurrentIndex(1);
}

void Widget::on_pushButton_hist_clicked()
{
	// Changes the Page to HIST when the button Hist is clicked
    ui->stackedWidgetNavigate->setCurrentIndex(2);
}

void Widget::on_pushButton_turn_clicked()
{
	// Changes the Page to TURN when the button Turn is clicked
    ui->stackedWidgetNavigate->setCurrentIndex(3);
}

// Login authentication (1 = Success, 0 = Failure)
int Widget::authLogin(){
	// Creates an Event Loop for the connection
    QEventLoop eventLoop;
	// Uses the Server URL for the connection and creates the request.
    QUrl serviceUrl = QUrl("http://batata.dlinkddns.com/back-end/php/login.php");  // Login .php
    QNetworkRequest request(serviceUrl);
	// Creates a JSON object to insert the necessary info to authenticate the
    // login on the server.
    QJsonObject json_login;
    json_login.insert("registry",Widget::userlogin);
    json_login.insert("password",Widget::password);
    json_login.insert("fromApp",1);
    QJsonDocument jsonDoc(json_login);
    QByteArray jsonData= jsonDoc.toJson();
	// Sets up the request.
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
	// Creates a new network manager to manage the connection and establishes the link.
    QNetworkAccessManager networkManager;
    QWidget::connect(&networkManager, SIGNAL(finished(QNetworkReply*)),&eventLoop, SLOT(quit()));
	// Posts the JSON and the request, and waits for an answer.
    QNetworkReply *reply = networkManager.post(request, jsonData);
	//Runs the loop until .quit() or .finished() is called.
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError)
    {
		//If there's no connection error it reads the server reply.
        QString strReply = (QString)reply->readAll();
        if (strReply == "Authentication error - User not found")
        {
			// If the User wasn't found, it updates the ui and returns 0.
            ui->progressBar->hide();
            ui->login_labelLoginStatus->show();
            ui->login_labelLoginStatus->clear();
            ui->login_labelLoginStatus->setText("Matrícula ou Senha Incorretos");
            return 0;
        }
        else if (strReply == "Login Successful")
        {
			// Else if the login was successful it returns 1.
            ui->progressBar->hide();
            ui->login_labelLoginStatus->show();
            ui->login_labelLoginStatus->clear();
            return 1;
        }
        else
        {
			// Else something unnexpected occurs, it returns 0.
            ui->progressBar->hide();
            ui->login_labelLoginStatus->show();
            ui->login_labelLoginStatus->clear();
            ui->login_labelLoginStatus->setText("Erro Interno");
            return 0;
        }
		// Then it leaves the eventLoop and deletes the reply.
        eventLoop.quit();
        delete reply;
    }
    else
    {
		// If the connection failed, it displays "Sem Conexão" and returns 0.
        ui->progressBar->hide();
        ui->login_labelLoginStatus->show();
        ui->login_labelLoginStatus->clear();
        ui->login_labelLoginStatus->setText("Sem Conexão");
        ui->login_labelLoginStatus->setStyleSheet("font-weight: bold; color: red");
        delete reply;
        return 0;
    }
}

// Function to implement a milliseconds delay.
void delayms( int millisecondsToWait ){
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

// Method that deals with the captcha and the connection with the board.
void Widget::on_turn_connect_clicked(){
	// Here it was implemented with a default destination IP, but the user can also change it
	// through the interface because in the tests the board didn't have a static IP. When the
	// project gets properly installed in a fixed location, this part of the code may be
	// simplified in order to leave out the IP setting configuration.
	
	// The default IP:
    if ((ui->turn_IP_field->text()=="")||(ui->turn_port_field->text()=="")){
        mSocket->connectToHost("192.168.25.6", 80); //Local Host's Address
        ui->turn_IP_status->setText("Usando 192.168.25.6:80");
    }
    else{
		// Else, if the user inputs an IP, the said IP'll be used.
        mSocket->connectToHost(ui->turn_IP_field->text(), ui->turn_port_field->text().toInt()); //Local Host's Address
        ui->turn_IP_status->setText("Usando "+ui->turn_IP_field->text()+":"+ui->turn_port_field->text());
    }

	// It waits for 500ms for a connection.
    mSocket->waitForConnected(500);
    if ((mSocket == nullptr)||(mSocket->state() != QTcpSocket::ConnectedState)){
		// If it wasn't Successful, it returns so in the interface.
        ui->turn_IP_status->setText("IP ou porta inválidos");
        return;
    }
    else{
		// Else it returns a Success.
        ui->turn_IP_status->setText("Conectou");
    }
	
	// Then it streams the user tag number + the captcha (inserted by the user) through the socket.
    QTextStream captcha(mSocket);
    captcha << Widget::tagNumber+ui->turn_captcha_field->text();
    mSocket->flush();

	// It waits for a response.
    connect(mSocket, &QTcpSocket::readyRead, [&]()
    {
        QTextStream T(mSocket);
        auto text = T.readAll();

        if (text == "y")
        {
			// If the captcha is correct, it receives "y" and displays a Success.
            ui->turn_status->setText("Acesso Liberado");
            delayms(100);
            QThread::msleep(5000);
        }
        else if (text == "n")
        {
			// If the captcha not correct, it receives "n" and displays a Capcha failure.
            ui->turn_status->setText("Captcha Incorreto");
            delayms(100);
            QThread::msleep(1000);
        }
        else if (text == "ooc")
        {
			// If the user lacks sufficient credits, it receives "ooc" (out of credits)
			// and displays a Credits failure.
            ui->turn_status->setText("Saldo Insuficiente");
            delayms(100);
            QThread::msleep(1000);
        }
        else if (text == "unr")
        {
			// If the user isn't registred, it receives "unr" (user not registred)
			// and displays a User failure.
            ui->turn_status->setText("Usuário Não Cadastrado");
            delayms(100);
            QThread::msleep(1000);
        }
		
        delayms(500);
		// After the connection, or if the response doesn't happen in time, the 
		// ui is reset, and the connection gets closed
        ui->turn_status->setText("Espere Sua Vez");
        ui->turn_IP_status->setText("Não conectado");
        mSocket->disconnectFromHost();
        mSocket->close();
        return;
    });

}

void Widget::on_turn_IP_field_editingFinished(){
	// UI update when the user finishes editing the IP.
    ui->turn_IP_status->setText("Usando "+ui->turn_IP_field->text()+":"+ui->turn_port_field->text());
}

void Widget::on_turn_port_field_editingFinished(){
	// UI update when the user finishes editing the Port.
    ui->turn_IP_status->setText("Usando "+ui->turn_IP_field->text()+":"+ui->turn_port_field->text());
}

void Widget::on_turn_IP_field_returnPressed(){
	// UI update when the user finishes editing the IP.
    ui->turn_IP_status->setText("Usando "+ui->turn_IP_field->text()+":"+ui->turn_port_field->text());
}

void Widget::on_turn_port_field_returnPressed(){
	// UI update when the user finishes editing the Port.
    ui->turn_IP_status->setText("Usando "+ui->turn_IP_field->text()+":"+ui->turn_port_field->text());
}

void Widget::on_turn_captcha_field_returnPressed(){
	// When the user presses "Return" while in the captcha text field, it calls the connection.
    Widget::on_turn_connect_clicked();
}


// Method to request the user info from the server:
void Widget::requestUser(){
	// The steps are basically the same as the authLogin() method.
    QEventLoop eventLoop;
    QUrl serviceUrl = QUrl("http://batata.dlinkddns.com/back-end/php/loadUsers.php");  // Users .php
    QNetworkRequest request(serviceUrl);
    QJsonObject json_user;
	// It only inserts the user login (matricula), which is the only info necessary post on the server.
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
		
		// Here the response is in JSON, so it parses it
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonArray json_array = jsonResponse.array();
		// And for each JSON array (there's only one in this case) it executes the loop.
        foreach (const QJsonValue & value, json_array)
        {
			// It converts the values to a JSON object.
            QJsonObject objuser = value.toObject();
			// It reads the tagnumber (an int) and represents it as a 8 digits string (filling with zeros)
            Widget::tagNumber = QString("%1").arg(objuser["tagNumber"].toString().toInt(), 8, 10, QChar('0'));
            // It reads the APP Credits (String)
			Widget::creditsApp = objuser["cellphoneCredit"].toString();
			// The Card Credits (String)
            Widget::creditsCard = objuser["cardCredit"].toString();
			// The Student's Name (String)
            Widget::username = objuser["name"].toString();
			// And it updates the ui.
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

// Method to request the user history (logs) from the server:
void Widget::requestLogs(){
	// First it clears the Events table
    ui->hist_tableWidgetEvents->setRowCount(0);
	// Then it connects with the server (same as the requestUser() method).
    QEventLoop eventLoop;
    QUrl serviceUrl = QUrl("http://batata.dlinkddns.com/back-end/php/loadLogs.php"); // Logs .php
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
		// If there's no reply error, it reads the reply.
        QString strReply = (QString)reply->readAll();
		// It puts the reply into a JSON Document and later converts it into a JSON array
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonArray json_array = jsonResponse.array();
		// It resets the number of events.
        Widget::events_counter = -1;
		// And for each event of the array, it runs the loop.
        foreach (const QJsonValue & value, json_array)
        {
			// It increments the counter (used to fill the Event table, working as the line index)
            Widget::events_counter++;
			// Inserts a new row on the Event table
            ui->hist_tableWidgetEvents->insertRow(ui->hist_tableWidgetEvents->rowCount());
			// Converts the values to a JSON Object
            QJsonObject objuser = value.toObject();
			// Overwrites the previous "eventos" (from the Event class), to work with the new event.
            Widget::eventos.setAll(objuser);
			// Gets the event details from the "eventos" and updates the table
            ui->hist_tableWidgetEvents->setItem(Widget::events_counter,0,new QTableWidgetItem(Widget::eventos.getType()));
            ui->hist_tableWidgetEvents->setItem(Widget::events_counter,1,new QTableWidgetItem(Widget::eventos.getDiffApp()));
            ui->hist_tableWidgetEvents->setItem(Widget::events_counter,2,new QTableWidgetItem(Widget::eventos.getDiffCard()));
            ui->hist_tableWidgetEvents->setItem(Widget::events_counter,3,new QTableWidgetItem(Widget::eventos.getTime()));
			// Changes the table row color (red = not resolved, green = resolved)
            for (int i=0;i<=3;i++){
                ui->hist_tableWidgetEvents->item(Widget::events_counter,i)->setBackgroundColor(Widget::eventos.getResolved());
                ui->hist_tableWidgetEvents->item(Widget::events_counter,i)->setTextAlignment(Qt::AlignCenter);
            }
        }
		// The "foreach" loop runs until the last event on the server response, so the last event stored in the "eventos" object
		// is the last event that happened. This is used later to update the "last event" on the info page.
        eventLoop.quit();
        delete reply;
    }
    else
    {
        delete reply;
    }
	// It updates the last sync time.
    ui->hist_labelLastUpdated->setText(QDateTime::currentDateTime().toString("hh:mm dd/MM"));
}

// Method to request the amount of people inside the RU
void Widget::requestInsideRU(){
	// Again, the request is similar from the other HTTP connections
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QWidget::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl( QString("http://batata.dlinkddns.com/back-end/php/loadAdminInfo.php") ) );
	// But now it is used a "get" instead of a "post" because the server doesn't need anything to reply the
	// number of people inside the RU.
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" or "quit()" has been called
    if (reply->error() == QNetworkReply::NoError) {
		// If there's no error in the reply it reads it all, parses the JSON reply,
		// converts it, and updates the ui with the number of people inside the RU.
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

// Method called when the Refresh button (in the info page) is pressed.
void Widget::on_info_pushButtonRefresh_clicked(){
	// It calls four already described methods:
    Widget::requestUser();
    Widget::requestInsideRU();
    Widget::requestLogs();
    Widget::updateLastEvent();
    return;
}

// Method called when the Refresh button (in the hist page) is pressed
void Widget::on_hist_pushButtonRefresh_clicked(){
	// It calls three already described methods and updates the UI.
    Widget::requestLogs();
    Widget::requestInsideRU();
    ui->hist_labelAPPCredit->setText(ui->info_labelAPPCred->text());
    ui->hist_labelCardCredit->setText(ui->info_labelCardCred->text());
    ui->hist_labelLastUpdated->setText(QDateTime::currentDateTime().toString("hh:mm dd/MM"));
    Widget::updateLastEvent();
}

void Widget::updateLastEvent(){
	// This method updates the last event on the info page, using the last "eventos" value kept from
	// the requestLogs() method.
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

// Method to add credits on the APP through the APP.
void Widget::addCredits(){
	// Again, it makes a HTTP request
    QEventLoop eventLoop;
    QUrl serviceUrl = QUrl("http://batata.dlinkddns.com/back-end/php/createCreditEvents.php"); // Insert Credits .php
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
		// If there's no error, it reads the reply.
        QString strReply = (QString)reply->readAll();
        if (strReply == "Update Successful! The credits will be updated as soon as the board syncs.")
        {
			// If the reply is successful, it returns a success message on the ui.
            ui->buy_labelStatus->setText("Sucesso!");
        }
        else
        {
			// Else, it updates the ui informing that an error occured.
            ui->buy_labelStatus->setText("Erro");
        }
        eventLoop.quit();
        delete reply;
    }
    else
    {
		// If there was a reply error, it returns a no connection message on the ui.
        ui->buy_labelStatus->setText("Sem Conexão");
        delete reply;
    }
}

// Method to convert the amount of credits to buy to Reais.
void Widget::on_buy_lineEditAmount_editingFinished(){
    Widget::buy_credits = ui->buy_lineEditAmount->text();
    double total_price = Widget::buy_credits.toDouble()*1.5;
	// After the conversion, it updates the UI.
    ui->buy_labelPrice->setText("R$ "+QString::number(total_price,'f',2));
}

// Method to handle the Buy button when clicked.
void Widget::on_buy_pushButtonBuy_clicked(){
    // It calls the addCredits() method (already explained) and updates the UI accordingly.
	ui->buy_labelStatus->setText("Processando");
    ui->buy_pushButtonBuy->hide();
    Widget::addCredits();
    delayms(1000);
    ui->buy_labelStatus->clear();
    ui->buy_pushButtonBuy->show();
}

