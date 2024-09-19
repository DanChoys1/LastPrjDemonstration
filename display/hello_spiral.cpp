#include "hello_spiral.h"

#include "geometry_math.h"
#include "coordinate_math.h"
#include "rect_point.h"
#include "uisettings.h"

#include <QPainter>

namespace
{
/// Скорость отрисовки всей спирали
constexpr qreal FillSectorSpeed = 360.0 / 2;

/// Цвета секций
const std::array<QColor,UISettings::HSColorsCount> Colors
//{
//    "#000000",
//    "#101667",
//    "#172d5c",
//    "#1b4b3e",
//    "#266e2f",
//    "#256c30",
//    "#42ae1a",
//    "#58cd1d",
//    "#72ec1c",
//    "#79f32a",
//    "#8eff42",
//    "#abff66",
//    "#d2ff99",
//    "#f2ffd0",
//    "#ffffff",
//    "#ffffff"
//};
= []()
{
    std::array<QColor,UISettings::HSColorsCount> colors;
    for(int i = 0; i < colors.size(); ++i)
        colors[i] = QColor(0, 255*(i/static_cast<qreal>(UISettings::HSColorsCount)), 0);

    return colors;
}();
}

HelloSpiral::HelloSpiral(qreal size, QGraphicsItem *p) :
    QGraphicsObject(p), _size(size), _img(_size, _size), _colorIt(Colors.cbegin()), _SectorHeight(_size/2 / UISettings::HSSectorPieCount)
{
    generateImg();

    connect(&_updateTimer, &QTimer::timeout, this, &HelloSpiral::updateBearing);
    _elTimer.start();
    _updateTimer.start(1000/30.0);
}

QRectF HelloSpiral::boundingRect() const
{
    return GeometryMath::centeredRectF(_size);
}

void HelloSpiral::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setPen(Qt::NoPen);

    painter->setBrush(Qt::black);
    painter->drawPie(-_size/2, -_size/2, _size, _size, 90*16, -360*16);

    QBrush brush(_img);
    brush.setTransform(QTransform().translate(_size/2, _size/2));
    painter->setBrush(brush);

    painter->drawPie(-_size/2, -_size/2, _size, _size, 90*16, -_currBearing*16);
}

void HelloSpiral::start()
{
    _elTimer.restart();
}

void HelloSpiral::generateImg()
{
    QPainter painter(&_img);
    painter.translate(_size/2, _size/2);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    for (int s = 0; s < UISettings::HSSectorsCount; ++s)
    {
        qreal a = (360.0 / UISettings::HSSectorsCount) * s;
        _colorIt = Colors.begin() + (s % UISettings::HSColorsCount);
        for(int i = 0; i < UISettings::HSSectorPieCount; ++i)
        {
            qreal minDist = _SectorHeight * i;
            qreal maxDist = _SectorHeight * (i+1);
            qreal bear1 = a;
            qreal bear2 = a + UISettings::HSSectorWide;

            RectPoint minPos = CoordinateMath::polarToSceneY(minDist, bear1);
            RectPoint maxPos = CoordinateMath::polarToSceneY(maxDist, bear2);

            QPainterPath area;
            area.moveTo(minPos);
            area.arcTo(GeometryMath::centeredRectF(minDist*2), 90 - bear1,  (bear1 - bear2));
            area.lineTo(maxPos);
            area.arcTo(GeometryMath::centeredRectF(maxDist*2), 90 - bear2, -(bear1 - bear2));
            area.lineTo(minPos);

            painter.setPen(*_colorIt);
            painter.setBrush(*_colorIt);
            painter.drawPath(area);

            if (++_colorIt == Colors.end())
                _colorIt = Colors.begin();
        }
    }
}

void HelloSpiral::updateBearing()
{
    _currBearing = FillSectorSpeed * (_elTimer.elapsed() / 1000.0);
    update();
}
