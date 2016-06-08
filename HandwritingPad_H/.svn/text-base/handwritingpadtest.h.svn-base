#ifndef HANDWRITINGPADTEST_H
#define HANDWRITINGPADTEST_H

#include <QtWidgets/QMainWindow>
#include "ui_handwritingpadtest.h"
#include "HandwritingPad.h"

#include <QTimer>

class HandwritingPadTest : public QMainWindow
{
    Q_OBJECT

public:
    HandwritingPadTest(QWidget *parent = 0);
    ~HandwritingPadTest();

public slots:
    void updateLabel();
    void setPen();
    void setEraser();
    void clear();

private:
    Ui::HandwritingPadTestClass ui;
    HandwritingPad* m_handwritingPad;
    QTimer m_updateLabelTimer;
};

#endif // HANDWRITINGPADTEST_H
