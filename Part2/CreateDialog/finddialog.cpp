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
    initianation();
    setDefaultSize();
    setDialogLayout();
}

FindDialog::~FindDialog()
{

}

void FindDialog::slotFindClicked()
{
    QString text = p_lineEdit->text();
    Qt::CaseSensitivity cs = p_caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
}

void FindDialog::slotEnableFindButton(const QString &text)
{

}

void FindDialog::initianation()
{
    resize(400, 100);
    this->setWindowTitle("Find");
    p_label = new QLabel;
    p_lineEdit = new QLineEdit;
    p_caseCheckBox = new QCheckBox;
    p_backwardCheckBox = new QCheckBox;
    p_findButton = new QPushButton;
    p_closeButton = new QPushButton;
    p_leftLayout = new QVBoxLayout;
    p_topLeftLayout = new QHBoxLayout;
    p_rightLayout = new QVBoxLayout;
    p_mainLayout = new QHBoxLayout(this);

    p_label->setText("Find &what");
    p_caseCheckBox->setText(tr("Match &case"));
    p_backwardCheckBox->setText(tr("Search &backward"));
    p_findButton->setText(tr("&Find"));
    p_closeButton->setText(tr("&Close"));

}

void FindDialog::setDialogLayout()
{
    setLayout(p_mainLayout);

    p_mainLayout->addLayout(p_leftLayout);
    p_mainLayout->addLayout(p_rightLayout);

    p_leftLayout->setSpacing(0);
    p_leftLayout->setMargin(0);
    p_leftLayout->setSizeConstraint(QLayout::SetMinimumSize);
    p_leftLayout->addLayout(p_topLeftLayout);
    p_leftLayout->addWidget(p_caseCheckBox);
    p_leftLayout->addWidget(p_backwardCheckBox);

    p_topLeftLayout->setSpacing(0);
    p_topLeftLayout->setMargin(0);
    p_topLeftLayout->addWidget(p_label);
    p_topLeftLayout->setSpacing(10);
    p_topLeftLayout->addWidget(p_lineEdit);

    p_rightLayout->setSpacing(0);
    p_rightLayout->setMargin(0);
//    p_rightLayout->addSpacing(25);
    p_rightLayout->addWidget(p_findButton);
//    p_rightLayout->addSpacing(20);
    p_rightLayout->addWidget(p_closeButton);
    p_rightLayout->addStretch(0);
}

void FindDialog::setDefaultSize()
{
    p_label->setMinimumSize(QSize(70, 30));
    p_lineEdit->setMinimumSize(QSize(130, 30));
    p_caseCheckBox->setMinimumSize(QSize(200,15));
    p_backwardCheckBox->setMinimumSize(QSize(200, 15));
    p_findButton->setMinimumSize(QSize(150, 30));
    p_closeButton->setMinimumSize(QSize(150, 30));
}
