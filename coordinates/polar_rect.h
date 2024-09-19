#ifndef POLAR_RECT_H
#define POLAR_RECT_H

#include "coordinate_math.h"

/// Полярный область
struct PolarRect
{
    /// Начало области
    PolarPoint p1;
    /// Конец области
    PolarPoint p2;

    constexpr PolarRect() = default;
    constexpr PolarRect(PolarPoint point1, PolarPoint point2) :
        p1(point1), p2(point2)
    {}

    constexpr PolarRect(qreal dist1, qreal bear1, qreal dist2, qreal bear2) :
        p1(dist1, bear1), p2(dist2, bear2)
    {}

    constexpr PolarRect(PolarPoint point, qreal distSize, qreal bearSize) :
        p1(point), p2(point.D + distSize, point.B + bearSize)
    {}

    /// Перемещение центра области в точку pos
    void setCenterPos(PolarPoint pos)
    {
        PolarPoint center = { (p1.D + p2.D) / 2, (p1.B + p2.B) / 2 };
        p1 = pos - center;
        p2 = pos + center;
    }

    /// Размер области по дистанции
    qreal distSize() const
    {
        return qAbs(p1.D - p2.D);
    }

    /// Размер области по пеленгу
    qreal bearSize() const
    {
        return qAbs(p1.B - p2.B);
    }

    /// Позиция центра области
    PolarPoint center() const
    {
        return { (p1.D + p2.D) / 2, (p1.B + p2.B) / 2 };
    }

    /// Содержит ли область позицию PolarPoint { dist, bear }
    bool contains(qreal dist, qreal bear) const
    {
        return contains({dist, bear});
    }
    /// Содержит ли область позицию p
    bool contains(PolarPoint p) const
    {
        bool isBetweenBear = false;
        if (p1.B < p2.B)
            isBetweenBear = p1.B <= p.B && p.B <= p2.B;
        else
            isBetweenBear = p1.B <= p.B || p.B <= p2.B;

        bool isBetweenDist = false;
        if (p1.D < p2.D)
            isBetweenDist = p1.D <= p.D && p.D <= p2.D;
        else
            isBetweenDist = p2.D <= p.D && p.D <= p1.D;

        return isBetweenBear && isBetweenDist;
    }
};

#endif // POLAR_RECT_H
