#include "distance_rings.h"

#include <QPainter>
#include <cmath>

#include "geometry_math.h"
#include "uisettings.h"

DistanceRings::DistanceRings(qreal size, int count, QGraphicsItem* p) :
    QGraphicsItem(p), _size(size), _count(count)
{
    initSpace();
}

QRectF DistanceRings::boundingRect() const
{
    return { GeometryMath::centeredRectF(_size) };
}

void DistanceRings::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen({UISettings::DistanceRingsColor, UISettings::DistanceRingsWidth});
    for(int i = 1; i <= _count; ++i)
    {
        painter->drawEllipse(GeometryMath::centeredRectF({_space * i, _space * i}));
    }
}

int DistanceRings::count() const
{
    return _count;
}

void DistanceRings::setCount(int count)
{
    _count = count;
    initSpace();
}

void DistanceRings::initSpace()
{
    _space = _size / _count;
}

qreal DistanceRings::size() const
{
    return _size;
}

void DistanceRings::setSize(qreal size)
{
    _size = size;
    initSpace();
}
