#include "threadtest.h"
#include <iostream>
#include <QDebug>
#include <QHBoxLayout>
ThreadTest::ThreadTest(QWidget *parent)
    : QWidget(parent)
{
    threadA.setMessage("A");
    threadB.setMessage("B");

    pthreadAButton = new QPushButton("Start A");
    pthreadBButton = new QPushButton("Start B");
    quitButton = new QPushButton("Quit");

    pthreadAButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pthreadBButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    quitButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    quitButton->setDefault(true);

    QHBoxLayout* h_Layout = new QHBoxLayout(this);
    h_Layout->addStretch(0);
    h_Layout->addWidget(pthreadAButton);
    h_Layout->addStretch(0);
    h_Layout->addWidget(pthreadBButton);
    h_Layout->addStretch(0);
    h_Layout->addWidget(quitButton);
    h_Layout->addStretch(0);
    setLayout(h_Layout);

    connect(pthreadAButton, SIGNAL(clicked()), this, SLOT(startOrStopThreadA()));
    connect(pthreadBButton, SIGNAL(clicked()), this, SLOT(startOrStopThreadB()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
}

ThreadTest::~ThreadTest()
{

}

void ThreadTest::closeEvent(QCloseEvent *event)
{
    qDebug() << "clsoe event!";
    threadA.stop();
    threadB.stop();
    threadA.wait();
    threadB.wait();
    event->accept();
}

void ThreadTest::startOrStopThreadA()
{
    if(threadA.isRunning()) {
        threadA.stop();
        pthreadAButton->setText("Start A");
    } else {
        threadA.start();
        pthreadAButton->setText("Stop A");
    }
}

void ThreadTest::startOrStopThreadB()
{
    if(threadB.isRunning()) {
        threadB.stop();
        pthreadBButton->setText("Start B");
    } else {
        threadB.start();
        pthreadBButton->setText("Stop B");
    }
}


Thread::Thread()
{
    stopped = false;
}

void Thread::setMessage(const QString &message)
{
    messageStr = message;
}

void Thread::stop()
{
    stopped = true;
}

void Thread::run()
{
    while(!stopped)
        std::cerr << qPrintable(messageStr);
    stopped = false;
    std::cerr << std::endl;
}
