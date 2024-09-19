#include "loupe_display.h"

#include <QPainter>
#include "uisettings.h"
#include "geometry_math.h"

LoupeDisplay::LoupeDisplay(QGraphicsItem *p) :
    QGraphicsItem(p)
{}

QRectF LoupeDisplay::boundingRect() const
{
    return QRectF { CoordinateMath::polarToRectY(_rect.p1), CoordinateMath::polarToRectY(_rect.p2) };
}

void LoupeDisplay::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen({UISettings::SelectAreaColor, UISettings::SelectAreaWidth});
    painter->drawPath(_area);
}

void LoupeDisplay::setRect(PolarRect rect)
{
    _rect = rect;

    RectPoint minPos = CoordinateMath::polarToSceneY(_rect.p1.D, _rect.p1.B);
    RectPoint maxPos = CoordinateMath::polarToSceneY(_rect.p2.D, _rect.p2.B);

    _area = QPainterPath();
    _area.moveTo(minPos);
    _area.arcTo(GeometryMath::centeredRectF(_rect.p1.D*2), 90 - _rect.p1.B,  (_rect.p1.B - _rect.p2.B));
    _area.lineTo(maxPos);
    _area.arcTo(GeometryMath::centeredRectF(_rect.p2.D*2), 90 - _rect.p2.B, -(_rect.p1.B - _rect.p2.B));
    _area.lineTo(minPos);

    update();
}

PolarRect LoupeDisplay::rect() const
{
    return _rect;
}
