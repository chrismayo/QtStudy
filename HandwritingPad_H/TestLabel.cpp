#include "TestLabel.h"

#include <QDateTime>
#include <QtDebug>
#include <QPainter>

TestLabel::TestLabel(QWidget *parent)
    : QLabel(parent)
{

}

TestLabel::~TestLabel()
{

}

void TestLabel::paintEvent(QPaintEvent * event)
{
#ifndef ZYNQ
//    qDebug() << "TestLabel::paintEvent@" << QDateTime::currentDateTime();
#endif

    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 128, 128));

    return QLabel::paintEvent(event);
}
