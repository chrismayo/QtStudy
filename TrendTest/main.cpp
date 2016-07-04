#include "trendtest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrendTest w;
    w.show();

    return a.exec();
}
