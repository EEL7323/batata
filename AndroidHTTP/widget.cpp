#include "widget.h"
#include "ui_widget.h"
#include <QtNetwork>
#include <QUrl>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

 //   nam = new QNetworkAccessManager(this);
    //connect(ui->connectButton,SIGNAL(clicked()),this,SLOT(on_connectButton_clicked()));
    //connect(ui->clearButton,SIGNAL(clicked()),this,SLOT(on_clearButton_clicked()));
//    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
    //ui->JSONView->setPlainText("");
    ui->statusLabel->clear();
    ui->valuesView->setReadOnly(1);


}

Widget::~Widget()
{
    delete ui;
}


void Widget::sendRequest() {

    // create custom temporary event loop on stack
    QEventLoop eventLoop;


    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QWidget::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
/*
    // the HTTP request
    if ((ui->addressField->text()=="http://")||(ui->addressField->text()=="")){
        QNetworkRequest req( QUrl( QString("https://jsonplaceholder.typicode.com/posts") ) );
        QNetworkReply *reply = mgr.get(req);
    }
    else{
        QNetworkRequest req( QUrl( QString(ui->addressField->text()) ) );
        QNetworkReply *reply = mgr.get(req);
    }
*/
    QNetworkRequest req( QUrl( QString("https://jsonplaceholder.typicode.com/posts") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called


    if (reply->error() == QNetworkReply::NoError) {

        QString strReply = (QString)reply->readAll();

        //parse json
        //qDebug() << "Response:" << strReply;

        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        //QString jsonString = jsonResponse.toJson(QJsonDocument::Compact);
        QJsonArray json_array = jsonResponse.array();
        //qDebug() << json_array;

        if (ui->JSONView->toPlainText()==""){
            return;
        }

        QString allValues = ui->JSONView->toPlainText();
        QStringList valuein = allValues.split(" ");
        QString result = "";
        //qDebug() << "\n\n\n\n";
        //qDebug() << valuein;
        //qDebug() << valuein[0];
        //qDebug() << valuein[1];
        //qDebug() << "\n\n\n\n";
        int values_number = allValues.length() - allValues.replace(" ", "").length();
        foreach (const QJsonValue & value, json_array)
        {
            QJsonObject obj = value.toObject();
            //qDebug() << obj;

            for( int i = 0; i <= values_number;i++)
            {
                //QJsonObject json_obj = value.toObject();
                //qDebug() << value;

                //qDebug() << "\n\n"<<i<<"\n"<<values_number<<"\n";
                //QJsonObject json_obj = jsonResponse.object();
                //qDebug() << "1 :" << json_obj;
                //qDebug() << "2 :" << json_obj();
                //qDebug() << valuein[i] << json_obj[(valuein[i])].toString();
                //result = result+(valuein[i]+" : "+json_obj[valuein[i]].toString()+"\n");

                //qDebug() << valuein[i] +" " + obj[(valuein[i])].toString();
                result = result+(valuein[i]+" : "+obj[(valuein[i])].toString()+"\n");
                //qDebug() << json_obj["registryNumber"].toString();
            }
            result = result+"\n";
        }
        ui->valuesView->setText(result);
        eventLoop.quit();
        ui->statusLabel->setText("Success!");
        delete reply;
    }
    else {
        //failure
        //qDebug() << "Failure" <<reply->errorString();
        ui->statusLabel->setText("Failure :(");
        delete reply;
    }
}



void Widget::on_clearButton_clicked()
{
    ui->addressField->setText("http://");
    ui->JSONView->clear();
    ui->valuesView->clear();
    ui->statusLabel->clear();
}

void Widget::on_connectButton_clicked()
{
    ui->statusLabel->clear();
    sendRequest();
    return;
}

void Widget::on_autoFillButton_clicked()
{
    ui->addressField->setText("https://jsonplaceholder.typicode.com/posts");
    ui->statusLabel->clear();
    ui->valuesView->clear();
    ui->JSONView->setPlainText("title body id userId");
}
