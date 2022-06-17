#include "servertablo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    serverTablo w;
    w.show();
    return a.exec();
}
