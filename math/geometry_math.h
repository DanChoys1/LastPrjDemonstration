#ifndef GEOMETRYMATH_H
#define GEOMETRYMATH_H

#include <QPair>
#include <QPointF>
#include <QRectF>

namespace GeometryMath
{
    /// Расчет точек пересечения прямой и окружности
    /// lPoint  - точка принадлежащая прямой
    /// lAngle  - угол поворота прямой
    /// cPos    - позиция окружности
    /// cRadius - радиус окружности
    /// points  - точки пересечения
    /// return  - кол-во пересечений
    int lineCircleIntersections(QPointF lPoint, qreal lAngle, QPointF cPos, qreal cRadius, QPair<QPointF, QPointF>& points);

    /// Возвращает прямоугольник у которого центр в начале координат
    /// size - размер прямоугольника
    QRectF centeredRectF(QSizeF size);
    /// Возвращает прямоугольник у которого центр в начале координат
    /// width  - ширина прямоугольника
    /// height - высота прямоугольника
    QRectF centeredRectF(qreal width, qreal height);
    /// Возвращает прямоугольник у которого центр в начале координат
    /// size  - сторона квадрата
    QRectF centeredRectF(qreal size);

    /// Синус угла (Градусы)
    qreal sin(qreal a);
    /// Косинус угла (Градусы)
    qreal cos(qreal a);
    /// Тангенс угла (Градусы)
    qreal tan(qreal a);

    /// Перевод угла в [0,360)
    qreal to360(qreal angle);
    /// Перевод угла в (-180,180]
    qreal to180(qreal angle);

};

#endif // GEOMETRYMATH_H
