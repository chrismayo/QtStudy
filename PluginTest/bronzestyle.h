#ifndef BRONZESTYLE_H
#define BRONZESTYLE_H

#include <QCommonStyle>
#include <QPalette>
#include <QPainter>
#include <QIcon>
#include <QStyleOptionViewItem>
class BronzeStyle : public QCommonStyle
{
    Q_OBJECT
public:
     BronzeStyle() {}
     void polish(QPalette &palette);
     void polish(QWidget *widgt);
     void unpolish(QWidget *widget);

     int styleHint(StyleHint sh, const QStyleOption *opt, const QWidget *w, QStyleHintReturn *shret) const;
     int pixelMetric(PixelMetric m, const QStyleOption *opt, const QWidget *widget) const;
     void drawPrimitive(PrimitiveElement which, const QStyleOptionComplex* option, QPainter *painter, const QWidget *widget = 0) const;
     void drawComplexControl(ComplexControl whichControl, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = 0) const;
     QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *w) const;

public slots:
     QIcon standardIconImplementation(StandardPixmap witch, const QStyleOption* option, const QWidget* widget = 0) const;

private:
     void drawBronzeFrame(const QStyleOption* option, QPainter *painter) const;
     void drawBronzeBevel(const QStyleOption *option, QPainter * painter) const;
     void drawBronzeCheckBoxindicator(const QStyleOption* option, QPainter* painter) const;
     void drawBronzeSpinBoxButton(SubControl which, const QStyleOptionComplex* option, QPainter *painter) const;
};

#endif // BRONZESTYLE_H
