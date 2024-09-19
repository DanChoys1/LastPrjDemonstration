#include "auto_catch_area.h"
#include "coordinate_math.h"
#include "geometry_math.h"
#include "uisettings.h"

#include <QPainter>

AutoCatchArea::AutoCatchArea(QGraphicsItem *p) :
    QGraphicsItem(p)
{}

QRectF AutoCatchArea::boundingRect() const
{
    return _area.boundingRect();
}

void AutoCatchArea::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen({UISettings::SelectAreaColor, UISettings::SelectAreaWidth});
    painter->drawPath(_area);
}

qreal AutoCatchArea::distPos() const
{
    return _distPos;
}

void AutoCatchArea::setDistPos(qreal d)
{
    _distPos = d;
    update();
}

qreal AutoCatchArea::bearing() const
{
    return _bearPos;
}

void AutoCatchArea::setBearing(qreal b)
{
    _bearPos = b;
    update();
}

void AutoCatchArea::move(qreal dist, qreal bear)
{
    _distPos += dist;
    _bearPos += bear;
    update();
}
void AutoCatchArea::setSize(qreal dist, qreal bear)
{
    _distSize = dist;
    _bearSize = bear;
    if (_distSize < 0)
        _distSize = 0;
    if (_bearSize < 0)
        _bearSize = 0;
    update();
}

void AutoCatchArea::incrementSize(qreal dist, qreal bear)
{
    _distSize += dist;
    _bearSize += bear;
    if (_distSize < 0)
        _distSize = 0;
    if (_bearSize < 0)
        _bearSize = 0;
    update();
}

qreal AutoCatchArea::maxDistPos() const
{
    return _distPos + _distSize;
}

qreal AutoCatchArea::maxBearPos() const
{
    return _bearPos + _bearSize;;
}

void AutoCatchArea::update()
{
    qreal dist = maxDistPos();
    qreal bear = maxBearPos();
    RectPoint minPos = CoordinateMath::polarToSceneY(_distPos, _bearPos);
    RectPoint maxPos = CoordinateMath::polarToSceneY(dist, bear);

    _area = QPainterPath();
    _area.moveTo(minPos);
    _area.arcTo(GeometryMath::centeredRectF(_distPos*2), 90 - _bearPos,  (_bearPos - bear));
    _area.lineTo(maxPos);
    _area.arcTo(GeometryMath::centeredRectF(dist*2), 90 - bear, -(_bearPos - bear));
    _area.lineTo(minPos);
}
