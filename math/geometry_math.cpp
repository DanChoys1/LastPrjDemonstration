#include "geometry_math.h"

#include <QtMath>

int GeometryMath::lineCircleIntersections(QPointF lPoint, qreal lAngle, QPointF cPos, qreal cRadius, QPair<QPointF, QPointF>& points)
{
    lPoint -= cPos; // Считаем центром координат позицию круга

    // Ax + By + C = 0 <=> kx + (-1)y + b = 0
    qreal a = tan(lAngle);
    qreal a2 = a*a;
    qreal b = -1;
    qreal b2 = b*b;
    // y = kx + b -> b = y - kx
    qreal c = lPoint.y() - a * lPoint.x();
    qreal c2 = c*c;
    qreal r2 = cRadius*cRadius;

    qreal x0 = -a*c / (a2 + b2);
    qreal y0 = -b*c / (a2 + b2);
    if (c2 > r2 * (a2 + b2) + __DBL_EPSILON__)
    {
        return 0;
    }
    else if (qAbs(c2 - r2 * (a2 + b2)) < __DBL_EPSILON__)
    {
        points.first = QPointF { x0, y0 } + cPos;
        return 1;
    }
    else
    {
        qreal d = r2 - c2 / (a2+b2);
        qreal mult = qSqrt(d / (a2 + b2));

        points.first  = QPointF { x0 + b * mult, y0 - a * mult } + cPos;
        points.second = QPointF { x0 - b * mult, y0 + a * mult } + cPos;
        return 2;
    }
}

QRectF GeometryMath::centeredRectF(QSizeF size)
{
    return centeredRectF(size.width(), size.height());
}

QRectF GeometryMath::centeredRectF(qreal width, qreal height)
{
    return QRectF { -width / 2, -height / 2, width, height };
}

QRectF GeometryMath::centeredRectF(qreal size)
{
    return centeredRectF(size, size);
}

qreal GeometryMath::sin(qreal a)
{
    return qSin( qDegreesToRadians(a) );
}

qreal GeometryMath::cos(qreal a)
{
    return qCos( qDegreesToRadians(a) );
}

qreal GeometryMath::tan(qreal a)
{
    return qTan( qDegreesToRadians(a) );
}

qreal GeometryMath::to360(qreal angle)
{
    angle = std::fmod(angle, 360.0);
    if (angle < 0)
    {
        angle = 360 + angle;
    }

    return angle;
}

qreal GeometryMath::to180(qreal angle)
{
    angle = to360(angle);
    if (angle > 180)
    {
        angle = - 360 + angle;
    }

    return angle;
}
