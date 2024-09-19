#ifndef DISTANCERINGS_H
#define DISTANCERINGS_H

#include <QGraphicsItem>

/// Отрисовка колец дальности.
/// Расстояние между кольцами зависит от количества колец и размера области отображения
class DistanceRings : public QGraphicsItem
{
public:
    DistanceRings(qreal size, int count = 1, QGraphicsItem* p = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override;

    /// Количество колец дальности
    int count() const;
    /// Количество колец дальности
    void setCount(int count);

    /// Размер области отображения колец дальности
    qreal size() const;
    /// Размер области отображения колец дальности
    void setSize(qreal size);

private:
    /// Установить Отступ между кольцами
    void initSpace();

private:
    /// Область отображения
    qreal _size;
    /// Количество колец дальности
    int _count = 0;
    /// Отступ между кольцами
    qreal _space = 1;

};

#endif // DISTANCERINGS_H
