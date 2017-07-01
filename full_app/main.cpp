/*
 * Main file, to run the app.
 */
 
#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.showMaximized();

    return a.exec();
}
