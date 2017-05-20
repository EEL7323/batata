#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("windowsxp");
    Widget w;
    w.showMaximized();
    return a.exec();
}
