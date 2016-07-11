#include "threadtest.h"
#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>
ThreadTest::ThreadTest(QWidget *parent)
    : QWidget(parent)
    , p_btnStartA(NULL)
    , p_btnStartB(NULL)
    , p_btnQuit(NULL)
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    p_btnStartA = new QPushButton;
    p_btnStartB = new QPushButton;
    p_btnQuit =  new QPushButton;

    p_btnStartA->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    p_btnStartB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    p_btnQuit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    p_btnStartA->setText("StartA");
    p_btnStartB->setText("StartB");
    p_btnQuit->setText("Quit");

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addStretch(1);
    layout->addWidget(p_btnStartA);
    layout->addStretch(0);
    layout->addWidget(p_btnStartB);
    layout->addStretch(0);
    layout->addWidget(p_btnQuit);
    layout->addStretch(1);

    threadA.setMessage("A");
    threadB.setMessage("B");


    connect(p_btnStartA, SIGNAL(clicked()), this, SLOT(slotStartA()));
    connect(p_btnStartB, SIGNAL(clicked()), this, SLOT(slotStartB()));
    connect(p_btnQuit, SIGNAL(clicked()), this, SLOT(close()));
}

ThreadTest::~ThreadTest()
{

}

void ThreadTest::slotStartA()
{
    if(threadA.isRunning()) {
        threadA.stop();
        p_btnStartA->setText("Start A");
    } else {
        threadA.start();
        p_btnStartA->setText("Stop A");
    }
}

void ThreadTest::slotStartB()
{
    if(threadB.isRunning()) {
        threadB.stop();
        p_btnStartB->setText("Start B");
    } else {
        threadB.start();
        p_btnStartB->setText("Stop B");
    }
}


Thread::Thread()
{
    stopped = false;
}

void Thread::setMessage(const QString &message)
{
    meesgeStr =message;
}

void Thread::stop()
{
    mutex.lock();
    stopped = true;
    mutex.unlock();
}

void Thread::run()
{
    while (1) {
        mutex.lock();
        if(stopped) {
            stopped = false;
            mutex.unlock();
            break;
        }
        mutex.unlock();
        qDebug() << meesgeStr;
    }
    stopped = false;
}
