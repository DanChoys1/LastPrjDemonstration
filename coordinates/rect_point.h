#ifndef RECT_POINT_H
#define RECT_POINT_H

#include <QPointF>

/// Точка в прямоугольных координатах
struct RectPoint
{
    /// Абсцисса
    double x = 0;
    /// Ордината
    double y = 0;

    RectPoint() = default;
    RectPoint(double x, double y) :
        x(x), y(y)
    {}
    RectPoint(const QPointF& point)
    {
        x = point.x();
        y = point.y();
    }
    RectPoint(const RectPoint& other)
    {
        x = other.x;
        y = other.y;
    }

    RectPoint& operator =(const QPointF& point)
    {
        x = point.x();
        y = point.y();
        return *this;
    }
    RectPoint& operator =(const RectPoint& point)
    {
        x = point.x;
        y = point.y;
        return *this;
    }

    RectPoint& operator +=(const RectPoint& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    RectPoint& operator -=(const RectPoint& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    RectPoint operator +(const RectPoint& other) const
    {
        RectPoint copy = *this;
        copy += other;
        return copy;
    }

    RectPoint operator -(const RectPoint& other) const
    {
        RectPoint copy = *this;
        copy -= other;
        return copy;
    }

    RectPoint& operator *=(double scale)
    {
        x *= scale;
        y *= scale;
        return *this;
    }

    RectPoint& operator /=(double scale)
    {
        x /= scale;
        y /= scale;
        return *this;
    }

    RectPoint operator /(double scale) const
    {
        RectPoint r = *this;
        r /= scale;
        return r;
    }

    operator QPointF() const
    {
        return QPointF(x, y);
    }
};

#endif // RECT_POINT_H
