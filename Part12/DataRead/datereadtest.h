#ifndef DATEREADTEST_H
#define DATEREADTEST_H

#include <QWidget>

class DateReadTest : public QWidget
{
    Q_OBJECT

public:
    DateReadTest(QWidget *parent = 0);
    ~DateReadTest();

private:
    void parse(QString file_name);
};

#endif // DATEREADTEST_H
