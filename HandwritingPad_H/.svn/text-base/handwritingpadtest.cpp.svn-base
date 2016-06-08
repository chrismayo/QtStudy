#include "handwritingpadtest.h"

#include <QHBoxLayout>
#include <QDateTime>
#include <QColorDialog>

HandwritingPadTest::HandwritingPadTest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

#if 0
    // 如果直接禁用背景重绘，则效率更高，但各个窗口均需要手动重绘自己的背景，实现有一定困难
    setAttribute(Qt::WA_OpaquePaintEvent);
    ui.label->setAttribute(Qt::WA_OpaquePaintEvent);
#endif

    QHBoxLayout* layout = new QHBoxLayout(ui.label);
    m_handwritingPad = new HandwritingPad(ui.label);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(m_handwritingPad);

    QAction* undoAction = m_handwritingPad->history().createUndoAction(this, tr("Undo Glyph"));
    QAction* redoAction = m_handwritingPad->history().createRedoAction(this, tr("Redo Glyph"));
    QAction* setPenAction = new QAction(tr("Set Pen"), this);
    QAction* setEraserAction = new QAction(tr("Set Eraser"), this);
    QAction* clearAction = new QAction(tr("Clear"), this);
    ui.mainToolBar->addAction(undoAction);
    ui.mainToolBar->addAction(redoAction);
    ui.mainToolBar->addAction(setPenAction);
    ui.mainToolBar->addAction(setEraserAction);
    ui.mainToolBar->addAction(clearAction);

    connect(setPenAction, &QAction::triggered, this, &HandwritingPadTest::setPen);
    connect(setEraserAction, &QAction::triggered, this, &HandwritingPadTest::setEraser);
    connect(clearAction, &QAction::triggered, this, &HandwritingPadTest::clear);
    connect(&m_updateLabelTimer, &QTimer::timeout, this, &HandwritingPadTest::updateLabel);
    m_updateLabelTimer.start(100);
}

HandwritingPadTest::~HandwritingPadTest()
{
    delete m_handwritingPad;
    m_handwritingPad = NULL;
}

void HandwritingPadTest::updateLabel()
{
    QString text = QDateTime::currentDateTime().toString();
    ui.label->setText(text);
}

void HandwritingPadTest::setPen()
{
    QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Pen Color"), QColorDialog::ShowAlphaChannel);
    m_handwritingPad->setPen(QPen(QBrush(color), 5));
}

void HandwritingPadTest::setEraser()
{
    m_handwritingPad->setEraser(5);
}

void HandwritingPadTest::clear()
{
    m_handwritingPad->clearHistory();
    m_handwritingPad->clearBuffer();
}
