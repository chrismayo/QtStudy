#include "HandwritingPad.h"

#include <QPainter>
#include <QtDebug>
#include <QDateTime>
#include <QUndoCommand>


class HandwritingGlyphCommand : public QUndoCommand
{
public:
    HandwritingGlyphCommand(HandwritingPad* parent) 
        : QUndoCommand()
        , m_parent(parent)
        , m_preBuffer()
        , m_postBuffer()
    {
        Q_ASSERT(m_parent);
        if (m_parent)
        {
            QImage buffer = parent->buffer();
            m_preBuffer   = buffer;
            m_postBuffer  = buffer;
        }
    }

    void setPostBuffer(const QImage& buffer)
    {
        m_postBuffer = buffer;
    }

protected:
    virtual void undo() Q_DECL_OVERRIDE
    {
        m_parent->setBuffer(m_preBuffer);
    }

    virtual void redo() Q_DECL_OVERRIDE
    {
        m_parent->setBuffer(m_postBuffer);
    }

private:
    HandwritingPad* m_parent;
    QImage          m_preBuffer;            //!< 执行此命令之前的缓冲区状态（只读的隐式共享数据，不需要担心内存开销）
    QImage          m_postBuffer;           //!< 执行此命令之后的缓冲区状态（只读的隐式共享数据，不需要担心内存开销）
};


HandwritingPad::HandwritingPad(QWidget *parent)
    : QWidget(parent)
    , m_buffer(800, 600, QImage::Format_ARGB32_Premultiplied)
    , m_pen(QBrush(Qt::black), 5)
    , m_lastPoint(-1, -1)
    , m_history()
{
#ifdef _WIN32
    setWindowFlags(Qt::FramelessWindowHint);
#endif
    setAttribute(Qt::WA_TranslucentBackground);

    
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(230, 230, 255, 128)); // 默认的半透明背景色
    this->setPalette(palette);
    this->clearBuffer();
    this->m_history.setUndoLimit(5);
}

HandwritingPad::~HandwritingPad()
{

}


void HandwritingPad::setEraser(int width /*= 5*/)
{
    QColor background = this->palette().background().color();
    m_pen = QPen(background, width);
}


void HandwritingPad::clearBuffer()
{
    QColor background = this->palette().background().color();
    m_buffer.fill(background);
    update();
}

void HandwritingPad::clearHistory()
{
    while (m_history.canUndo())
    {
        m_history.undo();
    }

    m_history.clear();
}

void HandwritingPad::resizeEvent(QResizeEvent * event)
{
    m_buffer = m_buffer.scaled(event->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void HandwritingPad::paintEvent(QPaintEvent * event)
{
    QRect updateRect = event->rect();

    //qDebug() << "HandwritingPad::paintEvent(" << updateRect << ")@" << QDateTime::currentDateTime();

    QPainter painter(this);
    painter.drawImage(updateRect, m_buffer, updateRect);
}


void HandwritingPad::mousePressEvent(QMouseEvent * event)
{
    // 记录笔画到QUndoCommand
    m_history.push(new HandwritingGlyphCommand(this));

    QPoint pos = event->pos();

    QPainter bufferPainter(&m_buffer);
    bufferPainter.setPen(m_pen);
    bufferPainter.setCompositionMode(QPainter::CompositionMode_Source);
    bufferPainter.drawPoint(pos);
    bufferPainter.end();

    int penWidth = m_pen.width() + 2;
    QRect dirtyRect(pos.x() - penWidth/2, pos.y() - penWidth/2, penWidth, penWidth);
    update();

    m_lastPoint = pos;
}

void HandwritingPad::mouseMoveEvent(QMouseEvent * event)
{
    QPoint pos = event->pos();

    QPainter bufferPainter(&m_buffer);
    bufferPainter.begin(&m_buffer);
    bufferPainter.setPen(m_pen);
    bufferPainter.setCompositionMode(QPainter::CompositionMode_Source);
    bufferPainter.drawLine(m_lastPoint, pos);
    bufferPainter.end();

    int penWidth   = m_pen.width() + 2;
    int lastPointX = m_lastPoint.x();
    int lastPointY = m_lastPoint.y();
    int posX       = pos.x();
    int posY       = pos.y();
    int left, top, right, bottom;
    if (lastPointX < posX)
    {
        left  = lastPointX;
        right = posX;
    }
    else
    {
        left  = posX;
        right = lastPointX;
    }
    if (lastPointY < posY)
    {
        top    = lastPointY;
        bottom = posY;
    }
    else
    {
        bottom = lastPointY;
        top    = posY;
    }
    QRect dirtyRect(left - penWidth/2, top - penWidth/2, right - left + penWidth, bottom - top + penWidth);
    update(dirtyRect);

    m_lastPoint = pos;
}

void HandwritingPad::mouseReleaseEvent(QMouseEvent * event)
{
    mouseMoveEvent(event);

    int count = m_history.count();
    if (count >= 1)
    {
        const QUndoCommand* lastCommand = m_history.command(count-1);
        HandwritingGlyphCommand* unfinishedGlyphCommand = (HandwritingGlyphCommand*)lastCommand;
        if (unfinishedGlyphCommand)
        {
            unfinishedGlyphCommand->setPostBuffer(m_buffer);
        }
    }
}

