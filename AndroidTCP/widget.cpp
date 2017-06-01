#include "widget.h"
#include "ui_widget.h"
#include <QTcpSocket>
#include <QTextStream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mSocket = new QTcpSocket(this);
    ui->captcha_field->setInputMethodHints(Qt::ImhPreferNumbers);
    ui->IP_field->setInputMethodHints(Qt::ImhFormattedNumbersOnly);
    ui->port_field->setInputMethodHints(Qt::ImhPreferNumbers);
    ui->IP_status->setText(" ");

    connect(mSocket, &QTcpSocket::readyRead, [&]() {
        QTextStream T(mSocket);
        auto text = T.readAll();

        if (text == "y"){
            ui->status->setText("Acesso Liberado");
        }
        else if (text == "n"){
            ui->status->setText("Captcha Incorreto");
        }
        else{
            ui->status->setText("Espere Sua Vez");
        }

    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_connect_clicked()
{
    if ((ui->IP_field->text()=="")||(ui->port_field->text()=="")){
        mSocket->connectToHost("192.168.43.195", 80); //Local Host's Address
        ui->IP_status->setText("Usando 192.168.43.195:80");
    }
    else{
        mSocket->connectToHost(ui->IP_field->text(),ui->port_field->text().toInt()); //Local Host's Address
    }

    if (mSocket == nullptr){
        return;
    }
    QTextStream captcha(mSocket);
    captcha << ui->captcha_field->text();
    mSocket->flush();

    QTextStream response(mSocket);
    auto text = response.readAll();

    if (text == "y"){
        ui->status->setText("Acesso Liberado");
    }
    else if (text == "n"){
        ui->status->setText("Captcha Incorreto");
    }
    else{
        ui->status->setText("Espere Sua Vez");
    }

}


void Widget::on_captcha_field_editingFinished()
{

    if ((ui->IP_field->text()=="")||(ui->port_field->text()=="")){
        mSocket->connectToHost("192.168.43.195", 80); //Local Host's Address
        ui->IP_status->setText("Usando 192.168.43.195:80");
    }
    else{
        mSocket->connectToHost(ui->IP_field->text(),ui->port_field->text().toInt()); //Local Host's Address
    }

    if (mSocket == nullptr){
        return;
    }
    QTextStream captcha(mSocket);
    captcha << ui->captcha_field->text();
    mSocket->flush();

    QTextStream response(mSocket);
    auto text = response.readAll();

    if (text == "y"){
        ui->status->setText("Acesso Liberado");
    }
    else if (text == "n"){
        ui->status->setText("Captcha Incorreto");
    }
    else{
        ui->status->setText("Espere Sua Vez");
    }
}

void Widget::on_IP_field_editingFinished()
{
    ui->IP_status->setText("Usando "+ui->IP_field->text()+":"+ui->port_field->text());
}

void Widget::on_port_field_editingFinished()
{
    ui->IP_status->setText("Usando "+ui->IP_field->text()+":"+ui->port_field->text());
}
