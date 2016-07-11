#ifndef THREADTEST_H
#define THREADTEST_H

#include <QWidget>
#include <QThread>
#include <QMutex>
class QPushButton;
//class Thread;
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
    QMutex mutex;
    QString meesgeStr;
    volatile bool stopped;
};

class ThreadTest : public QWidget
{
    Q_OBJECT

public:
    ThreadTest(QWidget *parent = 0);
    ~ThreadTest();

private slots:
    void slotStartA();
    void slotStartB();
private:
    Thread threadA;
    Thread threadB;
    QPushButton* p_btnStartA;
    QPushButton* p_btnStartB;
    QPushButton* p_btnQuit;
};

#endif // THREADTEST_H
