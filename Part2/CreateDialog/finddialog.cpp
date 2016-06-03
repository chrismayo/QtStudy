#include "finddialog.h"
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
    , p_label(NULL)
    , p_lineEdit(NULL)
    , p_caseCheckBox(NULL)
    , p_backwardCheckBox(NULL)
    , p_findButton(NULL)
    , p_closeButton(NULL)
{
    p_label = new QLabel;
    p_lineEdit = new QLineEdit;
    p_caseCheckBox = new QCheckBox;
    p_backwardCheckBox = new QCheckBox;
    p_findButton = new QPushButton;
}

FindDialog::~FindDialog()
{

}
