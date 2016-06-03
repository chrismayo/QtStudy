#ifndef THREADTEST_H
#define THREADTEST_H

#include <QWidget>
#include <QThread>
#include <QCloseEvent>
#include <QPushButton>
class Thread : public QThread
{
    Q_OBJECT
public:
    Thread();
    void setMessage(const QString &message);
    void stop();

protected:
    void run();

private:
    QString messageStr;
    volatile bool stopped;
};

class ThreadTest : public QWidget
{
    Q_OBJECT

public:
    ThreadTest(QWidget *parent = 0);
    ~ThreadTest();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void startOrStopThreadA();
    void startOrStopThreadB();

private:
    Thread threadA;
    Thread threadB;
    QPushButton* pthreadAButton;
    QPushButton* pthreadBButton;
    QPushButton* quitButton;
};

#endif // THREADTEST_H
