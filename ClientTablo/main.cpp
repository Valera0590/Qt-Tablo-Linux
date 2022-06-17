#include "clienttablo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    clientTablo w;
    w.show();
    return a.exec();
}
