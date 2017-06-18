#include "event.h"
#include <QColor>


Event::Event(){
}

Event::Event(QJsonObject eventObj){
    type = eventObj.value("type").toString();
    diffApp = eventObj.value("diffCredCellphone").toString();
    diffCard = eventObj.value("diffCredTag").toString();
    time = eventObj.value("time").toString();
    resolved = eventObj.value("toResolve").toString();

}

Event::Event(QString type1, QString diffApp1, QString diffCard1, QString time1){
    type = type1;
    diffApp = diffApp1;
    diffCard = diffCard1;
    time = time1;
}

QString Event::getTime(){ //"2017-01-01 00:00:01"
    QString hour = time;
    QString day = time;
    QString month = time;
    hour = hour.remove(0,11).remove(5,8);
    day = day.remove(0,8).remove(2,11);
    month = month.remove(0,5).remove(2,14);
    return hour+" "+day+"/"+month;
}

QString Event::getDiffApp(){
    if (diffApp =="")
        return "0";
    else
    return diffApp;
}

QString Event::getDiffCard(){
    if (diffCard =="")
        return "0";
    else
    return diffCard;
}


QString Event::getType(){

    QString event_name ="";

    switch (type.toInt()){
        case 0:
            event_name = "Novo\nCadastro";
            break;
        case 1:
            event_name = "Usuário\nRemovido";
            break;
        case 2:
            event_name = "Cadastro\nEditado";
            break;
        case 3:
            event_name = "Entrou no RU";
            break;
        case 4:
            event_name = "Saiu do RU";
            break;
        case 5:
            event_name = "Créditos\nAlterados";
            break;
        case 6:
            event_name = "Clear Logs"; //Server
            break;
        default: event_name = "Indefinido";

    }

    return event_name;
}

QColor Event::getResolved(){
    if (resolved == "1")
        return QColor(255, 0, 0, 120);
    else if (resolved == "0")
        return QColor(0, 255, 0, 120);
}

void Event::setAll(QJsonObject eventObj){
    type = eventObj.value("type").toString();
    diffApp = eventObj.value("diffCredCellphone").toString();
    diffCard = eventObj.value("diffCredTag").toString();
    time = eventObj.value("time").toString();
    resolved = eventObj.value("toResolve").toString();

}
