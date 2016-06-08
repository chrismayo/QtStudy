/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：HandWritePad.cpp
 * 概   要：提供手写功能的绘图版控件
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-8-5
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/

#include "handwritepad.h"
#include <QMouseEvent>
#include <QPushButton>
#include <QButtonGroup>
#include <QTime>
#include <QApplication>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QTouchEvent>

HandWritePad::HandWritePad(QWidget *parent)
    : QWidget(parent)
    , m_screenWidth(0), m_screenHeigh(0), m_penStyle(1), m_imageWidth(0)
    , m_imageHeigh(0), previusID(-1), previousPenChoice(-1)
    , p_imageWidget(NULL), p_rightWidget(NULL), p_paletteWidget(NULL)
    , p_cancelButton(NULL), p_penButton(NULL), p_paletteButton(NULL)
    , p_eraseButton(NULL), p_undoButton(NULL), p_clearButton(NULL)
    , p_indivButton(NULL), p_commonButton(NULL)
    , p_btnBlack1(NULL), p_btnBlack2(NULL), p_btnBlack3(NULL)
    , p_btnRed1(NULL), p_btnRed2(NULL), p_btnRed3(NULL)
    , p_btnBlue1(NULL), p_btnBlue2(NULL), p_btnBlue3(NULL)
    , p_groupPalette(NULL), p_groupMenu(NULL), p_penLabel(NULL)
    , p_paletteLabel(NULL), p_eraserLabel(NULL), p_undoLabel(NULL)
    , p_clearLabel(NULL), p_mainLayout(NULL), p_paletteLayout(NULL)
    , p_menuLayout(NULL), p_paletteStyleLayout(NULL)
{
    initialzation();
    setWinLayout();
    setMenu();      // 菜单按钮初始化
    setPaletteStyle();      // 画笔组按钮初始化
    connectFunC();
    setMyStyle();
}

HandWritePad *HandWritePad::instance()
{
    static HandWritePad handwirte;
    return &handwirte;
}

void HandWritePad::reSetdrawImage(int width, int height, QPoint pos, bool layout)
{
    m_imageWidth = width;
    m_imageHeigh = height;
    m_imagePos = pos;
    /** 设置背景颜色  **/
//    int color = DevConfigPtr->displayBasic.basicBackground;

    int color = 0;
    p_imageWidget->setBackgroundColor(color);
    p_imageWidget->setGeometry(m_imagePos.x(), m_imagePos.y(), m_imageWidth, m_imageHeigh);
    p_imageWidget->setReSetdrawImage(m_imageWidth, m_imageHeigh, m_imagePos);
    p_imageWidget->setscreenLaaoutValue(layout);
    p_undoButton->setEnabled(false);
    p_indivButton->setEnabled(false);
    p_commonButton->setEnabled(false);
    /** 根据当前布局判断手写板的布局  **/
    if(!layout) {
        p_cancelButton->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
        p_rightWidget->setMaximumSize(QSize(rect().width() / 9, rect().height()));
        p_rightWidget->setMinimumSize(QSize(rect().width() / 9, rect().height()));
        p_mainLayout->setDirection(QBoxLayout::LeftToRight);
        p_paletteLayout->setDirection(QBoxLayout::TopToBottom);
        p_menuLayout->setDirection(QBoxLayout::TopToBottom);
    } else {
        p_cancelButton->setMinimumWidth(p_penButton->width());
        p_commonButton->setMinimumWidth(p_penButton->width());
        p_indivButton->setMinimumWidth(p_penButton->width());
        p_rightWidget->setMaximumSize(QSize(rect().width(), rect().height() / 8));
        p_rightWidget->setMinimumSize(QSize(rect().width(), rect().height() / 8));
        p_mainLayout->setDirection(QBoxLayout::BottomToTop);
        p_paletteLayout->setDirection(QBoxLayout::LeftToRight);
        p_menuLayout->setDirection(QBoxLayout::LeftToRight);
    }

    slotGroupMenuClicked(1);
    slotGroupClicked(1);
}

HandWritePad::~HandWritePad()
{

}

void HandWritePad::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPainterPath path;
    path.setFillRule(Qt::OddEvenFill);
    path.addRect(e->rect());
    path.addRect(QRect(m_imagePos,QSize(m_imageWidth, m_imageHeigh)));
    painter.setPen(Qt::transparent);
    painter.setBrush(QColor(0,0,0,180));
    painter.drawPath(path);
}

void HandWritePad::slotGroupMenuClicked(int id)
{
    if(previusID != -1)
        p_groupMenu->button(previusID)->setChecked(false);
    previusID = id;
    p_groupMenu->button(previusID)->setChecked(true);

    switch (id) {
    case 0:         // cancel button clicked
        p_imageWidget->setClear();
        p_paletteWidget->setHidden(true);
        this->close();
        BackgroundPad::instance()->close();
        break;
    case 1:         // pen button clicked
        p_imageWidget->setPenStyle(m_penStyle);
        p_paletteWidget->setHidden(true);
        break;
    case 2:         // palette button clicked
        if(p_paletteWidget->isHidden()) {
            p_paletteWidget->setHidden(false);
        } else {
            p_paletteWidget->setHidden(true);
        }
        break;
    case 3:         // erase button clicked
        p_imageWidget->setPenStyle(9); // 9代表橡皮擦操作
        p_paletteWidget->setHidden(true);
        break;
    case 4:         // undo button clicked
        p_imageWidget->undoAction();
        p_paletteWidget->setHidden(true);
        break;
    case 5:         // clear button clicked
        p_imageWidget->clearAction();
        p_paletteWidget->setHidden(true);
        break;
    case 6:         // indiv save button clicked
        p_imageWidget->imageIndivSave();
        p_paletteWidget->setHidden(true);
        p_imageWidget->setClear();
        this->close();
        BackgroundPad::instance()->close();
        break;
    case 7:         // common save button clicked
        p_imageWidget->imageCommonSave();
        p_paletteWidget->setHidden(true);
        p_imageWidget->setClear();
        this->close();
        BackgroundPad::instance()->close();
    default:
        break;
    }
}

void HandWritePad::slotGroupClicked(int id)
{
    if(previousPenChoice != -1)
        p_groupPalette->button(previousPenChoice)->setChecked(false);
    previousPenChoice = id;
    p_groupPalette->button(previousPenChoice)->setChecked(true);

    p_paletteWidget->setHidden(true);
    switch (id) {
    case 0:     // 画笔风格：黑色 细
        m_penStyle = 0;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/black1.png"));
        break;
    case 1:     // 画笔风格：黑色 普通
        m_penStyle = 1;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/black2.png"));
        break;
    case 2:     // 画笔风格：黑色 粗
        m_penStyle = 2;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/black3.png"));
        break;
    case 3:     // 画笔风格：红色 细
        m_penStyle = 3;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/red1.png"));
        break;
    case 4:     // 画笔风格：红色 普通
        m_penStyle = 4;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/red2.png"));
        break;
    case 5:     // 画笔风格：红色 粗
        m_penStyle = 5;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/red3.png"));
        break;
    case 6:     // 画笔风格：蓝色 细
        m_penStyle = 6;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/blue1.png"));
        break;
    case 7:     // 画笔风格：蓝色 普通
        m_penStyle = 7;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/blue2.png"));
        break;
    case 8:     // 画笔风格：蓝色 粗
        m_penStyle = 8;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/blue3.png"));
        break;
    default:
        break;
    }

    p_imageWidget->setPenStyle(m_penStyle);
}

/*
 * 功能：
 * 	  检测到绘图版画面下押时，设置撤销按钮状态，隐藏画笔选择界面
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void HandWritePad::slotMousePressed()
{
    p_undoButton->setEnabled(true);
    p_paletteWidget->setHidden(true);
}

/*
 * 功能：
 * 	  当前画面已无内容或者撤销次数达到5次时，设置撤销按钮的状态
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void HandWritePad::slotImageCLear()
{
    p_undoButton->setEnabled(false);
}

/*
 * 功能：
 * 	  画面无任何操作时，设置保存按钮状态
 * 参数：
 * 	  bool
 * 返回值：
 * 	  无
 */
void HandWritePad::slotAbleAction(bool able)
{
    if(able) {
        p_indivButton->setEnabled(true);
        p_commonButton->setEnabled(true);
    } else {
        p_indivButton->setEnabled(false);
        p_commonButton->setEnabled(false);
    }
}

/*
 * 功能：
 * 	 界面初始化
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void HandWritePad::initialzation()
{
//    m_screenHeigh = DESKTOP_HEIGHT- (DESKTOP_HEIGHT/MAINWIN_STATUSBAR_HIGHT_SCALE);
//    m_screenWidth = DESKTOP_WIDTH;

    m_screenHeigh = QApplication::desktop()->height();
    m_screenWidth = QApplication::desktop()->width();
    resize(m_screenWidth, m_screenHeigh);               // 调整绘图版整体大小
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    p_imageWidget = new DrawArea(this);
    p_paletteWidget = new QWidget(this);
    p_groupPalette = new QButtonGroup(this);
    p_groupMenu = new QButtonGroup(this);

    p_rightWidget = new QWidget(this);
    p_cancelButton = new QPushButton(p_rightWidget);
    p_penButton = new QPushButton(p_rightWidget);
    p_paletteButton = new QPushButton(p_rightWidget);
    p_eraseButton = new QPushButton(p_rightWidget);
    p_undoButton = new QPushButton(p_rightWidget);
    p_clearButton = new QPushButton(p_rightWidget);
    p_indivButton = new QPushButton(p_rightWidget);
    p_commonButton = new QPushButton(p_rightWidget);
    p_penLabel = new QLabel(p_penButton);
    p_paletteLabel = new QLabel(p_paletteButton);
    p_eraserLabel = new QLabel(p_eraseButton);
    p_undoLabel = new QLabel(p_undoButton);
    p_clearLabel = new QLabel(p_clearButton);
    p_btnBlack1 = new QPushButton(p_paletteWidget);
    p_btnBlack2 = new QPushButton(p_paletteWidget);
    p_btnBlack3 = new QPushButton(p_paletteWidget);
    p_btnRed1 = new QPushButton(p_paletteWidget);
    p_btnRed2 = new QPushButton(p_paletteWidget);
    p_btnRed3 = new QPushButton(p_paletteWidget);
    p_btnBlue1 = new QPushButton(p_paletteWidget);
    p_btnBlue2 = new QPushButton(p_paletteWidget);
    p_btnBlue3 = new QPushButton(p_paletteWidget);
    p_mainLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    p_paletteLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    p_menuLayout = new QBoxLayout(QBoxLayout::TopToBottom, p_rightWidget);
    p_paletteStyleLayout = new QGridLayout(p_paletteWidget);
    p_paletteWidget->setHidden(true);
    p_paletteWidget->setMaximumSize(QSize(rect().width() / 3, rect().height() / 3));
    p_paletteWidget->setMinimumSize(QSize(rect().width() / 3, rect().height() / 3));
    p_rightWidget->setMaximumSize(QSize(rect().width() / 9, rect().height()));
    p_rightWidget->setMinimumSize(QSize(rect().width() / 9, rect().height()));

    p_rightWidget->setObjectName("RightWIdget");
}

void HandWritePad::connectFunC()
{
    connect(p_groupMenu, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupMenuClicked(int)));
    connect(p_groupPalette, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupClicked(int)));
    connect(p_imageWidget, SIGNAL(sigImageClear()), this, SLOT(slotImageCLear()));
    connect(p_imageWidget, SIGNAL(sigMousePressed()), this, SLOT(slotMousePressed()));
    connect(p_imageWidget, SIGNAL(sigImageSave(QImage*,QString&,quint8,QPoint&)),\
            this, SIGNAL(sigHWMsg(QImage*,QString&,quint8,QPoint&)));
    connect(p_imageWidget, SIGNAL(sigAbleAction(bool)), this, SLOT(slotAbleAction(bool)));
}

void HandWritePad::setMyStyle()
{
    setStyleSheet("QWidget{background-color: rgb(255, 255, 255);}"\
                  "#RightWIdget{background-color: rgb(245, 245, 245);}"\
                  "QLabel{font-family:'Microsoft YaHei';font-size:17px;}"\
                  "QPushButton{background-color: rgb(230, 230, 230);border-width: "\
                                                    "1px; border-style: solid; border-color: rgb(200, 200, 200);"\
                  "font-family:'Microsoft YaHei';font-size:17px;}"\
                  "QPushButton:pressed{background-color: rgb(179, 231, 255);border-width: "\
                  "1px; border-style: solid; border-color: rgb(179, 231, 255);"\
                  "font-family:'Microsoft YaHei';font-size:17px;}"\
                  "QPushButton:checked{background-color: rgb(179, 231, 255);border-width: "\
                  "1px; border-style: solid; border-color: rgb(220, 244, 255);"\
                  "font-family:'Microsoft YaHei';font-size:17px;}");
}

void HandWritePad::setMenu()
{
    p_groupMenu->addButton(p_cancelButton, 0);
    p_groupMenu->addButton(p_penButton, 1);
    p_groupMenu->addButton(p_paletteButton, 2);
    p_groupMenu->addButton(p_eraseButton, 3);
    p_groupMenu->addButton(p_undoButton, 4);
    p_groupMenu->addButton(p_clearButton, 5);
    p_groupMenu->addButton(p_indivButton, 6);
    p_groupMenu->addButton(p_commonButton, 7);

    p_cancelButton->setText(tr("Cancel"));
    p_penButton->setIcon(QIcon(":/palette/Image/Palette/black2.png"));
    p_paletteButton->setIcon(QIcon(":/palette/Image/Palette/palette.png"));
    p_eraseButton->setIcon(QIcon(":/palette/Image/Palette/erase.png"));
    p_undoButton->setIcon(QIcon(":/palette/Image/Palette/undo.png"));
    p_clearButton->setIcon(QIcon(":/palette/Image/Palette/delette.png"));

    for(int i = 0; i < p_groupMenu->buttons().size(); ++i) {
        p_groupMenu->button(i)->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
        p_groupMenu->button(i)->setMinimumHeight(rect().height() / 11);
        p_groupMenu->button(i)->setMaximumHeight(rect().height() / 10);
        p_groupMenu->button(i)->setCheckable(true);
        p_groupMenu->button(i)->setFocusPolicy(Qt::NoFocus);
        p_groupMenu->button(i)->setIconSize(QSize(p_groupMenu->button(i)->width(), p_groupMenu->button(i)->height()));
    }

    p_penLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    p_penLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    p_penLabel->setText(tr("Pen"));
    p_penLabel->setGeometry(0, 0, p_penButton->width(), p_penButton->height());


    p_paletteLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    p_paletteLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    p_paletteLabel->setText(tr("Palette"));
    p_paletteLabel->setGeometry(0, 0, p_paletteButton->width(), p_paletteButton->height());

    p_eraserLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    p_eraserLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    p_eraserLabel->setText(tr("Eraser"));
    p_eraserLabel->setGeometry(0, 0, p_eraseButton->width(), p_eraseButton->height());

    p_undoButton->setEnabled(false);
    p_undoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    p_undoLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    p_undoLabel->setText(tr("Undo"));
    p_undoLabel->setGeometry(0, 0, p_undoButton->width(), p_undoButton->height());

    p_clearLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    p_clearLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    p_clearLabel->setText(tr("Clear"));
    p_clearLabel->setGeometry(0, 0, p_clearButton->width(), p_clearButton->height());

    p_indivButton->setText(tr("individual\nsaving"));
    p_commonButton->setText(tr("common\nsaving"));
}

void HandWritePad::setPaletteStyle()
{
    p_groupPalette->addButton(p_btnBlack1, 0);
    p_groupPalette->addButton(p_btnBlack2, 1);
    p_groupPalette->addButton(p_btnBlack3, 2);
    p_groupPalette->addButton(p_btnRed1, 3);
    p_groupPalette->addButton(p_btnRed2, 4);
    p_groupPalette->addButton(p_btnRed3, 5);
    p_groupPalette->addButton(p_btnBlue1, 6);
    p_groupPalette->addButton(p_btnBlue2, 7);
    p_groupPalette->addButton(p_btnBlue3, 8);

    p_groupPalette->button(0)->setIcon(QIcon(":/palette/Image/Palette/black1.png"));
    p_groupPalette->button(1)->setIcon(QIcon(":/palette/Image/Palette/black2.png"));
    p_groupPalette->button(2)->setIcon(QIcon(":/palette/Image/Palette/black3.png"));
    p_groupPalette->button(3)->setIcon(QIcon(":/palette/Image/Palette/red1.png"));
    p_groupPalette->button(4)->setIcon(QIcon(":/palette/Image/Palette/red2.png"));
    p_groupPalette->button(5)->setIcon(QIcon(":/palette/Image/Palette/red3.png"));
    p_groupPalette->button(6)->setIcon(QIcon(":/palette/Image/Palette/blue1.png"));
    p_groupPalette->button(7)->setIcon(QIcon(":/palette/Image/Palette/blue2.png"));
    p_groupPalette->button(8)->setIcon(QIcon(":/palette/Image/Palette/blue3.png"));

    for(int i = 0; i < p_groupPalette->buttons().size(); ++i) {
        p_groupPalette->button(i)->setFocusPolicy(Qt::NoFocus);
        p_groupPalette->button(i)->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
        p_groupPalette->button(i)->setCheckable(true);
        p_groupPalette->button(i)->setIconSize(QSize(p_groupPalette->button(i)->width()*2, p_groupPalette->button(i)->height()*2));
    }
}

void HandWritePad::setWinLayout()
{
    setLayout(p_mainLayout);
    p_mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    p_mainLayout->setSpacing(0);
    p_mainLayout->setMargin(0);
    p_mainLayout->addStretch();
    p_mainLayout->addLayout(p_paletteLayout);
    p_mainLayout->addWidget(p_rightWidget);

    p_paletteLayout->setSpacing(0);
    p_paletteLayout->setMargin(0);
    p_paletteLayout->addStretch(0);
    p_paletteLayout->addWidget(p_paletteWidget);
    p_paletteLayout->addStretch(0);

    p_menuLayout->setSpacing(0);
    p_menuLayout->setMargin(0);
    p_menuLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    p_menuLayout->addWidget(p_cancelButton);
    p_menuLayout->addStretch(0);
    p_menuLayout->addWidget(p_penButton);
    p_menuLayout->addWidget(p_paletteButton);
    p_menuLayout->addWidget(p_eraseButton);
    p_menuLayout->addWidget(p_undoButton);
    p_menuLayout->addWidget(p_clearButton);
    p_menuLayout->addStretch(0);
    p_menuLayout->addWidget(p_indivButton);
    p_menuLayout->addWidget(p_commonButton);

    p_paletteStyleLayout->setSpacing(0);
    p_paletteStyleLayout->setMargin(1);
    p_paletteStyleLayout->addWidget(p_btnBlack1, 0, 0, 1, 1);
    p_paletteStyleLayout->addWidget(p_btnBlack2, 0, 1, 1, 1);
    p_paletteStyleLayout->addWidget(p_btnBlack3, 0, 2, 1, 1);
    p_paletteStyleLayout->addWidget(p_btnRed1, 1, 0, 1, 1);
    p_paletteStyleLayout->addWidget(p_btnRed2, 1, 1, 1, 1);
    p_paletteStyleLayout->addWidget(p_btnRed3, 1, 2, 1, 1);
    p_paletteStyleLayout->addWidget(p_btnBlue1, 2, 0, 1, 1);
    p_paletteStyleLayout->addWidget(p_btnBlue2, 2, 1, 1, 1);
    p_paletteStyleLayout->addWidget(p_btnBlue3, 2, 2, 1, 1);
}

/*
 * 功能：
 * 	 自定义控件，在此控件上进行绘图，界面刷新时单独刷
 *   新该界面，减少刷新的时间消耗
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
DrawArea::DrawArea(QWidget *parent)
    : QWidget(parent)
    , m_firstDraw(true)
    , m_screenLayout(false)
    , m_choicePen(1)
    , m_imageWidth(0)
    , m_imageHeigh(0)
    , m_undoCount(0)
    , m_backgroundColor(0)
    , m_bigX(0)
    , m_bigY(0)
    , m_smallX(0)
    , m_smallY(0)
    , p_painterImage(NULL)
{
    m_pointVector.clear();
    this->setAttribute(Qt::WA_AcceptTouchEvents);
}

/*
 * 功能：
 * 	 设置画笔风格
 * 参数：
 * 	  int
 * 返回值：
 * 	  无
 */
void DrawArea::setPenStyle(int style)
{
    m_choicePen = style;
}

/*
 * 功能：
 * 	 撤销操作
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void DrawArea::undoAction()
{
    m_commonImage.operator =(m_imageSave.at(m_imageSave.size() - 1));   // 把上一个画面赋给当前画面
    p_painterImage = &m_commonImage;            // 指针指向当前画面
    update();
    m_imageSave.removeAt(m_imageSave.size() - 1);       // 删除数组中的当前画面
    if(m_imageSave.size() == 0)
        emit sigImageClear();
}

/*
 * 功能：
 * 	 画面清空操作
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void DrawArea::clearAction()
{
    emit sigMousePressed();
    m_commonImage.operator =(*p_painterImage);      // 保存清空前的画面
    m_undoCount = m_imageSave.size();
    if(m_undoCount > 4) {
        m_imageSave.removeAt(0);
    } else {
        // do nothing
    }
    m_imageSave.append(m_commonImage);

    m_clearImage = QImage(m_imageWidth, m_imageHeigh, QImage::Format_ARGB32_Premultiplied);
    p_painterImage = &m_clearImage;
    update();
}

/*
 * 功能：
 * 	 画面个别保存操作
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void DrawArea::imageIndivSave()
{
    // 设置图片的保存路径
//    QString dir = p_gCfgTrendGroup->getHWritePictureSaveDir() +
//            QString::number(p_gCfgTrendGroup->getHWritePictureIndex());
    QString dir = "/home/mayo/Image";

    if (m_screenLayout) {
        dir += QString("-%1").arg(m_smallX*100/m_imageWidth);
        QTransform trans;
        trans.rotate(90);
        QImage image = p_painterImage->copy(m_smallX - 2, m_smallY - 2, m_bigX - m_smallX + 4, m_bigY - m_smallY + 4);
        image = image.transformed(trans);
        if(image.save( dir, "PNG") == false) {
            qDebug() << "save error";
        }

        QPoint point(m_bigX, rect().height()-m_smallY);
        emit(sigImageSave(&image,dir, 1, point));
    } else {
        dir += QString("-%1").arg(m_smallY*100/m_imageHeigh);
        QImage image = p_painterImage->copy(m_smallX - 2, m_smallY - 2, m_bigX - m_smallX + 4, m_bigY - m_smallY + 4);
        if(image.save(dir, "PNG") == false) {
            qDebug() << "save error";
        }

        QPoint point(m_bigX, m_bigY);
        emit(sigImageSave(&image,dir, 1, point));
    }

    /** 设置历史趋势  **/
//    p_gCfgTrendGroup->setWorkMode(RealTimeRecord);
}

/*
 * 功能：
 * 	 画面共通保存操作
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void DrawArea::imageCommonSave()
{
    // 设置图片的保存路径
//    QString dir = p_gCfgTrendGroup->getHWritePictureSaveDir() +
//            QString::number(p_gCfgTrendGroup->getHWritePictureIndex());
    QString dir = "/home/mayo/Image";
    if (m_screenLayout) {
        dir += QString("-%1").arg(m_smallX*100/m_imageWidth);
        QTransform trans;
        trans.rotate(90);
        QImage image = p_painterImage->copy(m_smallX - 2, m_smallY - 2, m_bigX - m_smallX + 4, m_bigY - m_smallY + 4);
        image = image.transformed(trans);
        if (image.save(dir, "PNG") == false) {
            qDebug() << "save error";
        }

        QPoint point(m_bigX, rect().height()-m_smallY);
        emit(sigImageSave(&image,dir, 0, point));

    } else {
        dir += QString("-%1").arg(m_smallY*100/m_imageHeigh);
        QImage image = p_painterImage->copy(m_smallX - 2, m_smallY - 2, m_bigX - m_smallX + 4, m_bigY - m_smallY + 4);
        if(image.save(dir, "PNG") == false) {
            qDebug() << "save error";
        }

        QPoint point(m_bigX, m_bigY);
        emit(sigImageSave(&image,dir, 0, point));
    }

//    p_gCfgTrendGroup->setWorkMode(RealTimeRecord);
}

/*
 * 功能：
 * 	 设置绘图区域的大小
 * 参数：
 * 	  int width, int height, QPoint pos
 * 返回值：
 * 	  无
 */
void DrawArea::setReSetdrawImage(int width, int height, QPoint pos)
{
    m_imageWidth = width;
    m_imageHeigh = height;
    m_imagePos = pos;
    m_firstDraw = true;
    m_imageSave.clear();
    setPenStyle(1);
    m_commonImage = QImage(m_imageWidth, m_imageHeigh, QImage::Format_ARGB32_Premultiplied);
    m_commonImage.fill(QColor(255, 255, 255, 0));
    p_painterImage = &m_commonImage;
}

/*
 * 功能：
 * 	 清空参数的值
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void DrawArea::setClear()
{
    m_pointVector.clear();
    m_bigX = 0;
    m_bigY = 0;
    m_smallX = 0;
    m_smallY = 0;
}

void DrawArea::setBackgroundColor(int color)
{
    m_backgroundColor = color;
}

void DrawArea::setscreenLaaoutValue(bool layout)
{
    m_screenLayout = layout;
}

DrawArea::~DrawArea()
{

}

/*
 * 功能：
 * 	 触摸板操作事件
 * 参数：
 * 	  QEvent *event
 * 返回值：
 * 	  无
 */
bool DrawArea::event(QEvent *event)
{
    QTouchEvent *pevent;
    switch (event->type()){
    case QEvent::TouchBegin:
        pevent = static_cast<QTouchEvent *>(event);
        foreach (QTouchEvent::TouchPoint point, pevent->touchPoints()) {
            m_pointVector.append(point.pos());
        }
        pictureSave(m_pointVector.last());
        event->accept();
        return true;
    case QEvent::TouchCancel:
    case QEvent::TouchEnd:
//        pevent = static_cast<QTouchEvent *>(event);
//        foreach (QTouchEvent::TouchPoint point, pevent->touchPoints()) {
//            m_pointVector.append(point.pos());
//        }
//        paintTracePath(m_pointVector, m_choicePen);
        m_pointVector.clear();
        event->accept();
        return true;
    case QEvent::TouchUpdate:
        pevent = static_cast<QTouchEvent *>(event);
        foreach (QTouchEvent::TouchPoint point, pevent->touchPoints()) {
            m_pointVector.append(point.pos());
        }
        if((qreal)m_pointVector.last().x() > 0 && (qreal)m_pointVector.last().x() < m_imageWidth\
                && (qreal)m_pointVector.last().x() > m_bigX) {
            m_bigX = (qreal)m_pointVector.last().x();
        }

        if((qreal)m_pointVector.last().x() > 0 && (qreal)m_pointVector.last().x() < m_imageWidth\
                && (qreal)m_pointVector.last().x() < m_smallX) {
            m_smallX = (qreal)m_pointVector.last().x();
        }

        if((qreal)m_pointVector.last().y() > 0 && (qreal)m_pointVector.last().y() < m_imageHeigh
                && (qreal)m_pointVector.last().y() > m_bigY) {
            m_bigY = (qreal)m_pointVector.last().y();
        }

        if((qreal)m_pointVector.last().y() > 0 && (qreal)m_pointVector.last().y() < m_imageHeigh
                && (qreal)m_pointVector.last().y() < m_smallY) {
            m_smallY = (qreal)m_pointVector.last().y();
        }
        paintTracePath(m_pointVector, m_choicePen);
        event->accept();
        return true;
    default:
        break;
    }

    return QWidget::event(event);
}

void DrawArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0,0,*p_painterImage);
}

void DrawArea::mousePressEvent(QMouseEvent *event)
{
    if(event->x() > 0 && event->y() > 0 && event->x() < m_imageWidth\
            && event->y() < m_imageHeigh) {
        pictureSave(event->pos());      // 保存图片以及开始绘图
    }
}

void DrawArea::mouseMoveEvent(QMouseEvent *event)
{
    if(event->x() > 0 && event->y() > 0 && event->y() < m_imageHeigh && \
            event->x() < m_imageWidth) {
        m_pointVector.append(QPointF(event->pos()));
        if(m_bigX < m_pointVector.last().x())
            m_bigX = m_pointVector.last().x();
        if(m_bigY < m_pointVector.last().y())
            m_bigY = m_pointVector.last().y();
        if(m_smallX > m_pointVector.last().x())
            m_smallX = m_pointVector.last().x();
        if(m_smallY > m_pointVector.last().y())
            m_smallY = m_pointVector.last().y();
        paintTracePath(m_pointVector, m_choicePen);
    }
}

void DrawArea::mouseReleaseEvent(QMouseEvent */*event*/)
{
    m_pointVector.clear();
}

/*
 * 功能：
 * 	 在图片上进行绘图
 * 参数：
 * 	  QVector<QPointF> &points, int penChoice
 * 返回值：
 * 	  无
 */
void DrawArea::paintTracePath(QVector<QPointF> &points, int penChoice)
{
    QPainter painter(p_painterImage);
    setPainter(painter, penChoice);
    painter.drawPolyline(points.constData(), points.count());
    this->update();
    QPointF point = m_pointVector.last();
    m_pointVector.clear();
    m_pointVector.append(point);
}

/*
 * 功能：
 * 	 设置画笔的风格
 * 参数：
 * 	  QPainter &painter, int penChoice
 * 返回值：
 * 	  无
 */
void DrawArea::setPainter(QPainter &painter, int penChoice)
{
    // 判断用户所选择的画笔颜色以及粗细
    switch (penChoice) {
    case Choice_BlackSlim:
        if(0 == m_backgroundColor) {
            painter.setPen(QPen(QBrush(QColor(0x40, 0x40, 0x40)), 2));
        } else {
            painter.setPen(QPen(QBrush(QColor(255, 255, 255)), 2));
        }
        break;
    case Choice_BlackCommon:
        if(0 == m_backgroundColor) {
            painter.setPen(QPen(QBrush(QColor(0x40, 0x40, 0x40)), 4));
        } else {
            painter.setPen(QPen(QBrush(QColor(255, 255, 255)), 4));
        }
        break;
    case Choice_BlackThick:
        if(0 == m_backgroundColor) {
            painter.setPen(QPen(QBrush(QColor(0x40, 0x40, 0x40)), 6));
        } else {
            painter.setPen(QPen(QBrush(QColor(255, 255, 255)), 6));
        }
        break;
    case Choice_RedSlim:
        painter.setPen(QPen(Qt::red, 2));
        break;
    case Choice_RedCommon:
        painter.setPen(QPen(Qt::red, 4));
        break;
    case Choice_RedThick:
        painter.setPen(QPen(Qt::red, 6));
        break;
    case Choice_BlueSlim:
        painter.setPen(QPen(Qt::blue, 2));
        break;
    case Choice_BlueCommon:
        painter.setPen(QPen(Qt::blue, 4));
        break;
    case Choice_BlueThick:
        painter.setPen(QPen(Qt::blue, 6));
        break;
    case Choice_Eraser:
        painter.setPen(QPen(QBrush(QColor(255,255,255,0)), 25));
        painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
        break;
    default:
        break;
    }
}

/*
 * 功能：
 * 	 保存图片并记录最大值与最小值
 * 参数：
 * 	  QPointF point
 * 返回值：
 * 	  无
 */

void DrawArea::pictureSave(QPointF point)
{
    if(m_firstDraw) {
        m_bigX = m_smallX = (qreal)point.x();
        m_bigY = m_smallY = (qreal)point.y();
        m_firstDraw = false;
        emit sigAbleAction(true);
    }
    emit sigMousePressed();
    m_commonImage.operator =(*p_painterImage);
    m_undoCount = m_imageSave.size();
    if(5 == m_undoCount) {
        m_imageSave.removeAt(0);
    } else {
        // do nothing
    }

    m_imageSave.append(m_commonImage);
    m_pointVector.append(QPointF(point));
    paintTracePath(m_pointVector, m_choicePen);
}


BackgroundPad *BackgroundPad::instance()
{
    static BackgroundPad instance;
    return &instance;
}

void BackgroundPad::setHight(int h)
{
    this->resize(QApplication::desktop()->width(), h);
}

BackgroundPad::BackgroundPad(QWidget */*parent*/)
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

void BackgroundPad::paintEvent(QPaintEvent */*ev*/)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 0));
}
