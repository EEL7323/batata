#include "widget.h"
#include "ui_widget.h"
#include <QTcpSocket>
#include <QTextStream>
#include <QTime>
#include <QThread>
#include <QHostAddress>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mSocket = new QTcpSocket(this);
    ui->captcha_field->setInputMethodHints(Qt::ImhPreferNumbers);
    ui->IP_field->setInputMethodHints(Qt::ImhFormattedNumbersOnly);
    ui->port_field->setInputMethodHints(Qt::ImhPreferNumbers);
//    ui->IP_status->setText(" ");
    ui->IP_status->setText("Não conectado");
//    mSocket->waitForDisconnected(5000);
//    ui->status->setText("Espere Sua Vez");


}

Widget::~Widget()
{
    delete ui;
}

void delayms( int millisecondsToWait ){
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}


void Widget::on_connect_clicked()
{

    if ((ui->IP_field->text()=="")||(ui->port_field->text()=="")){
        mSocket->connectToHost("192.168.25.6", 80); //Local Host's Address
        ui->IP_status->setText("Usando 192.168.25.6:80");
    }
    else{
        //QHostAddress ip_address = QHostAddress(ui->IP_field->text());
        mSocket->connectToHost(ui->IP_field->text(), ui->port_field->text().toInt()); //Local Host's Address
        ui->IP_status->setText("Usando "+ui->IP_field->text()+":"+ui->port_field->text());
    }

    mSocket->waitForConnected(500);
    if ((mSocket == nullptr)||(mSocket->state() != QTcpSocket::ConnectedState)){
        ui->IP_status->setText("IP ou porta inválidos");
        return;
    }
    else{
        ui->IP_status->setText("Conectou");
    }
    QTextStream captcha(mSocket);
    captcha << ui->captcha_field->text();
    mSocket->flush();

    connect(mSocket, &QTcpSocket::readyRead, [&]() {
        QTextStream T(mSocket);
        auto text = T.readAll();

        if (text == "y"){
            ui->status->setText("Acesso Liberado");
            delayms(100);
            QThread::msleep(5000);

           }
        else if (text == "n"){
            ui->status->setText("Captcha Incorreto");
            delayms(100);
            QThread::msleep(1000);

        }
        delayms(500);
        ui->status->setText("Espere Sua Vez");
        mSocket->disconnectFromHost();
        mSocket->close();
        return;

    });

}

void Widget::on_IP_field_editingFinished()
{
    ui->IP_status->setText("Usando "+ui->IP_field->text()+":"+ui->port_field->text());
}

void Widget::on_port_field_editingFinished()
{
    ui->IP_status->setText("Usando "+ui->IP_field->text()+":"+ui->port_field->text());
}

void Widget::on_IP_field_returnPressed()
{
    ui->IP_status->setText("Usando "+ui->IP_field->text()+":"+ui->port_field->text());
}

void Widget::on_port_field_returnPressed()
{
    ui->IP_status->setText("Usando "+ui->IP_field->text()+":"+ui->port_field->text());
}

void Widget::on_captcha_field_returnPressed()
{
    Widget::on_connect_clicked();
}
