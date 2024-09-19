#ifndef LOUPEDISPLAY_H
#define LOUPEDISPLAY_H

#include <QGraphicsItem>

#include "polar_rect.h"

/// Отрисовка области лупы
class LoupeDisplay : public QGraphicsItem
{
public:
    LoupeDisplay(QGraphicsItem* p = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /// Прямоугольник области лупы
    void setRect(PolarRect rect);
    /// Прямоугольник области лупы
    PolarRect rect() const;

private:
    /// Прямоугольник области лупы
    PolarRect _rect;
    /// Отрисовываемая область
    QPainterPath _area;

};

#endif // LOUPEDISPLAY_H
