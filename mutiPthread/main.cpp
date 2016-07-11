#include "threadtest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ThreadTest w;
    w.show();

    return a.exec();
}
