#ifndef HANDWRITINGPAD_H
#define HANDWRITINGPAD_H

#include <QWidget>
#include <QResizeEvent>
#include <QPen>
#include <QImage>
#include <QVector>
#include <QUndoStack>

class HandwritingPad : public QWidget
{
    Q_OBJECT

public:
    HandwritingPad(QWidget *parent);
    ~HandwritingPad();

    /*!  
     * Getter/Setter for pen
     */
    const QPen& pen() const 
    { 
        return m_pen; 
    }
    void setPen(const QPen& pen) 
    { 
        m_pen = pen; 
    }
    void setEraser(int width = 5);

    /*!  
     * Getter/Setter for buffer
     */
    QImage buffer() const 
    { 
        return m_buffer; 
    }
    void setBuffer(const QImage& buffer) 
    { 
        m_buffer = buffer;
        update();
    }
    void clearBuffer();

    /*!  
     * Getter/Setter for history
     */
    QUndoStack& history()
    { 
        return m_history; 
    }
    void clearHistory();

protected:
    void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

private:
    QImage          m_buffer;               //!< 以图片形式保存需要绘制的点，提升绘制大量曲线时的效率
    QPen            m_pen;                  //!< 绘制手写曲线使用的画笔
    QPoint          m_lastPoint;            //!< 最近一次有效点击的坐标，用于与本次点击连线绘制，若该坐标无效说明为第一次点击
    QUndoStack      m_history;              //!< 笔画历史，可以撤销/重做
};

#endif // HANDWRITINGPAD_H
