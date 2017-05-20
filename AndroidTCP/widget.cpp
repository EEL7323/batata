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
        ui->listWidget->addItem(text);
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_connect_clicked()
{
    mSocket->connectToHost("150.162.198.214", 3333); //Local Host's Address
}
