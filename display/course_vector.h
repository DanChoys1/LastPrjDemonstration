#ifndef COURSEVECTOR_H
#define COURSEVECTOR_H

#include <QGraphicsItem>

#include "rect_point.h"

/// Отрисовка линии курса и пути пройденнего носителем за заданное время при заданной скорости
class CourseVector : public QGraphicsItem
{
    struct SpeedArrow
    {
        QLineF leftLine;
        QLineF rightLine;
    };

public:
    CourseVector(qreal size, QGraphicsItem* p = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;

    /// Размер области отрисовки
    qreal size() const;
    /// Размер области отрисовки
    void setSize(qreal size);

    /// Курс
    qreal course() const;
    /// Скорость
    qreal speed() const;
    /// Время за которое отображается пройденный путь носителя
    qreal moveVecTime() const;
    /// Время за которое отображается пройденный путь носителя
    RectPoint moveVec() const;

    //// Обновление данных
    void update(qreal course, qreal speed, qreal moveVecTime);
    //// Обновление данных
    void updateSpeed(qreal speed);
    //// Обновление данных
    void updateCourse(qreal course);
    //// Обновление данных
    void updateMoveVecTime(qreal time);

    /// Отображать ли путь который пройден носитель
    void setMovVecVisible(bool isVisible);

private:
    /// Размер области отрисовки
    qreal _size;

    /// Курс
    qreal _course = 0;
    /// Скорость
    qreal _speed = 0;
    /// Время за которое отображается путь который пройдет носитель
    qreal _moveVecTime = 0;

    /// Линия отрисовка курса
    QLineF _lineVector;
    /// Стрелка пути который пройдет носитель
    SpeedArrow _speedArrow;

    /// Флаг отображения пути
    bool _isMoveVecVisible = false;

};

#endif // COURSEVECTOR_H
