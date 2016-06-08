#include "handwritepad.h"
#include <QApplication>
#include <QPoint>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HandWritePad* hPad = HandWritePad::instance();
    hPad->reSetdrawImage(800, 600, QPoint(0, 0), false);
    hPad->show();

    return a.exec();
}
