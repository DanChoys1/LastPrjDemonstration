#include "situation_group.h"
#include "geometry_math.h"
#include "coordinate_math.h"
#include "display_settings.h"

#include <cmath>

SituationGroup::SituationGroup(qreal size, QGraphicsItem* p) :
    QGraphicsItem(p)
{
    setFlag(GraphicsItemFlag::ItemClipsChildrenToShape);
    setSize(size);
}

QRectF SituationGroup::boundingRect() const
{
    return _rect;
}

void SituationGroup::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
{}

QPainterPath SituationGroup::shape() const
{
    return _shapePath;
}

qreal SituationGroup::size() const
{
    return _size;
}

void SituationGroup::setSize(qreal size)
{
    _size = size;
    _rect = GeometryMath::centeredRectF(_size);
    _shapePath = {};
    _shapePath.addEllipse(_rect);

    update();
}

void SituationGroup::setPos(QPointF pos)
{
    _sitGroup.setPos(pos);
}

QPointF SituationGroup::moveBy(QPointF movePos)
{
    QPointF recPos = pos() + movePos;
    PolarPoint polPos = CoordinateMath::rectToPolarY(recPos);
    if (std::fmod(polPos.D, _size / 2) > _size / 2 * DisplaySettings::MaxCenterShiftCoeff)
    {
        polPos.B = GeometryMath::to360(polPos.B + 180);
        recPos = CoordinateMath::polarToRectY(polPos);
    }

    setPos(recPos);

    return pos();
}

QPointF SituationGroup::pos() const
{
    return _sitGroup.pos();
}

void SituationGroup::setRotation(qreal a)
{
    _sitGroup.setRotation(a);
}

qreal SituationGroup::rotation() const
{
    return _sitGroup.rotation();
}

void SituationGroup::addToGroup(QGraphicsItem *item)
{
    _sitGroup.addToGroup(item);
}
