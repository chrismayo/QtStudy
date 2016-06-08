#include "handwritingpadtest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HandwritingPadTest w;
    w.show();
    return a.exec();
}
