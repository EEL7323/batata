#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->stackedWidgetLogin->setCurrentIndex(0);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButtonLogin_clicked()
{
    ui->stackedWidgetLogin->setCurrentIndex(1);
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
