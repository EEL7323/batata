#ifndef EVENT_H
#define EVENT_H
#include <QString>
#include <QJsonObject>
#include <QColor>

class Event
{
public:
    Event();
    Event(QJsonObject);
    Event(QString, QString, QString, QString);
    QString getTime();
    QString getDiffApp();
    QString getDiffCard();
    QString getType();
    QColor getResolved();
    void setAll(QJsonObject);


private:
    QString time ="2017-01-01 00:00:01";
    QString diffApp ="0";
    QString diffCard ="0";
    QString type ="0";
    QString resolved = "1";


};

#endif // EVENT_H
