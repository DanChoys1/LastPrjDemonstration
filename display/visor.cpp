#include "visor.h"

#include <QPainter>

#include "geometry_math.h"
#include "uisettings.h"

Visor::Visor(qreal size, QGraphicsItem *p) :
    QGraphicsItem(p), _size(size)
{
    setSize(size);
    update({});
}

QRectF Visor::boundingRect() const
{
    return { GeometryMath::centeredRectF(_size) };
}

void Visor::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->translate(_translate);
    painter->setRenderHint(QPainter::Antialiasing);

    painter->setPen({UISettings::VisorColor, UISettings::VisorDistWidth});
    painter->drawEllipse(GeometryMath::centeredRectF({_pos.D*2, _pos.D*2}));

    if (_isShowParallelLines)
    {
        painter->drawLines({
                               QLineF{ QPointF{ _pos.D, _maxDist }, QPointF{ _pos.D, -_maxDist } },
                               QLineF{ QPointF{ -_pos.D, _maxDist }, QPointF{ -_pos.D, -_maxDist } }
                           });
    }

    painter->setPen(QPen(UISettings::VisorColor, UISettings::VisorBearWidth, Qt::PenStyle::DashLine));
    painter->drawLine(QPointF{0,0}, QPointF{ 0, -_maxDist });
}

PolarPoint Visor::pos() const
{
    return _pos;
}

void Visor::update(PolarPoint p)
{
    _pos = p;
    setRotation(p.B);
}

void Visor::move(PolarPoint p)
{
    p += pos();
    if (p.D < 0)
        p.D = 0;

    update(p);
}

void Visor::translate(RectPoint p)
{
    QGraphicsItem::setPos(p);
}

qreal Visor::size() const
{
    return _size;
}

void Visor::setSize(qreal size)
{
    _size = size;
    _maxDist = CoordinateMath::distance(_size / 2, _size / 2);
}

void Visor::toggleParallelLinesVisible()
{
    _isShowParallelLines = !_isShowParallelLines;
}
