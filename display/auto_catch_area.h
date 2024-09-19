#ifndef AUTOCATCHAREA_H
#define AUTOCATCHAREA_H

#include <QGraphicsItem>

/// Отрисовка области автозахвата
class AutoCatchArea : public QGraphicsItem
{
public:
    AutoCatchArea(QGraphicsItem* p = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;

    /// Позиция по дистанции
    qreal distPos() const;
    /// Позиция по дистанции
    void setDistPos(qreal d);

    /// Позиция по пеленгу
    qreal bearing() const;
    /// Позиция по пеленгу
    void setBearing(qreal b);

    /// Смещение позиции
    void move(qreal dist, qreal bear);

    /// Задает размер области
    void setSize(qreal dist, qreal bear);
    /// Увеличивает размер области
    void incrementSize(qreal dist, qreal bear);

    /// Позиция по дистанции второй точки области
    qreal maxDistPos() const;
    /// Позиция по пеленгу второй точки области
    qreal maxBearPos() const;

private:
    /// Обновление отрисовки
    void update();

private:
    /// Отрисовка
    QPainterPath _area;

    /// Позиция по дистанции
    qreal _distPos = 0;
    /// Позиция по пеленгу
    qreal _bearPos = 0;
    /// Размер области по дистанции
    qreal _distSize = 0;
    /// Размер области по пеленгу
    qreal _bearSize = 0;

};

#endif // AUTOCATCHAREA_H
