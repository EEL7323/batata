/*
 * Event Class Implementation
 */

#include "event.h"
#include <QColor>

// Empty constructor
Event::Event(){
}

// JSON-initialized constructor
Event::Event(QJsonObject eventObj){
	// JSON parsing, to take the values from the object, convert them to string,
	// and store them in the correct variable.
    type = eventObj.value("type").toString();
    diffApp = eventObj.value("diffCredCellphone").toString();
    diffCard = eventObj.value("diffCredTag").toString();
    time = eventObj.value("time").toString();
    resolved = eventObj.value("toResolve").toString();

}

// String-initialized constructor
Event::Event(QString type1, QString diffApp1, QString diffCard1, QString time1){
	// Passes them to the correct variable
    type = type1;
    diffApp = diffApp1;
    diffCard = diffCard1;
    time = time1;
}

// Method to convert and get the time from the format "YYYY-MM-DD hh:mm:ss"
// to "hh:mm DD-MM":
QString Event::getTime(){ //"2017-01-01 00:00:01"
    QString hour = time;
    QString day = time;
    QString month = time;
    hour = hour.remove(0,11).remove(5,8);
    day = day.remove(0,8).remove(2,11);
    month = month.remove(0,5).remove(2,14);
    return hour+" "+day+"/"+month;
}

// Method to get the variation on the APP credits
QString Event::getDiffApp(){
    if (diffApp =="")
        return "0";
    else
    return diffApp;
}

// Method to get the variation on the Card credits
QString Event::getDiffCard(){
    if (diffCard =="")
        return "0";
    else
    return diffCard;
}

// Method to get the type of the event (classified by flags)
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

// Method to get if the event was already resolved by the board or not.
// The value returned is a color, to set the row's color on the "Event 
// History" table for each event.
QColor Event::getResolved(){
    if (resolved == "1")
        // Semitransparent red
		return QColor(255, 0, 0, 120);
    else if (resolved == "0")
		// Semitransparent green
        return QColor(0, 255, 0, 120);
}


// Method to set all the variables in an already instanced Event
// from a JSON object (mainly used to fully overwrite an event or
// to input data on an empty Event).
void Event::setAll(QJsonObject eventObj){
    type = eventObj.value("type").toString();
    diffApp = eventObj.value("diffCredCellphone").toString();
    diffCard = eventObj.value("diffCredTag").toString();
    time = eventObj.value("time").toString();
    resolved = eventObj.value("toResolve").toString();

}
