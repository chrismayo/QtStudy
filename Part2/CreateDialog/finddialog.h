#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = 0);
    ~FindDialog();

signals:
    void findNext(const QString& str, Qt::CaseSensitivity vs);
    void finPrevious(const QString& str, Qt::CaseSensitivity vs);
private slots:
    void slotFindClicked();
    void slotEnableFindButton(const QString& text);

private:
    void initianation();
    void setDialogLayout();
    void setDefaultSize();
private:
    QLabel* p_label;
    QLineEdit* p_lineEdit;
    QCheckBox* p_caseCheckBox;
    QCheckBox* p_backwardCheckBox;
    QPushButton* p_findButton;
    QPushButton* p_closeButton;
    QVBoxLayout* p_leftLayout;
    QHBoxLayout* p_topLeftLayout;
    QVBoxLayout* p_rightLayout;
    QHBoxLayout* p_mainLayout;
};

#endif // FINDDIALOG_H
