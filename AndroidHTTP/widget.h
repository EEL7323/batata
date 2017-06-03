#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QMainWindow>
#include <QNetworkReply>
#include <QTextCodec>
#include <QtDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_connectButton_clicked();

    void on_clearButton_clicked();

    void sendRequest();

    //void finished(QNetworkReply *reply);

    void on_autoFillButton_clicked();

private:
    Ui::Widget *ui;
    QNetworkAccessManager *nam;
};



#endif // WIDGET_H
