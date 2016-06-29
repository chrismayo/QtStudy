#include "datereadtest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DateReadTest w;
    w.show();

    return a.exec();
}
