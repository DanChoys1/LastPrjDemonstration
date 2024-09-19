#ifndef VISOR_H
#define VISOR_H

#include "coordinate_math.h"

#include <QGraphicsItem>

/// Отрисовка визор
class Visor : public QGraphicsItem
{
public:
    Visor(qreal size, QGraphicsItem* p = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override;

    /// Позиция визора
    PolarPoint pos() const;

    /// Обновление отображаемой позиции визора
    void update(PolarPoint p);
    /// Перемещение позиции визора
    void move(PolarPoint p);
    /// Перемещение центра визора
    void translate(RectPoint p);

    /// Размер отображения визора
    qreal size() const;
    /// Размер отображения визора
    void setSize(qreal size);

    /// Перелключить отображение параллельных линий
    void toggleParallelLinesVisible();

private:
    /// Отрисовываемый круг дальности
    QRectF _distCircle;
    /// Отрисовываемая линия пеленга
    QLineF _bearLine;

    /// Заданная позиция
    PolarPoint _pos;

    /// Смещение центра визора
    RectPoint _translate;

    /// Размер отображения визора
    qreal _size = 0;
    /// Максимальная дистанция дальности визора
    qreal _maxDist = 0;

    /// Флаг отображения параллельных линий
    bool _isShowParallelLines = false;

};

#endif // VISOR_H
