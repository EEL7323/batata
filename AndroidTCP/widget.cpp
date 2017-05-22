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
    mSocket->connectToHost("192.168.43.195", 80); //Local Host's Address

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
