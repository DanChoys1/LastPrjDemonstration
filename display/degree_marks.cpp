#include "degree_marks.h"

#include <QPainter>
#include <QtMath>
#include <QGraphicsScene>

#include "coordinate_math.h"
#include "geometry_math.h"
#include "uisettings.h"
#include "display_setting_enums.h"

using namespace DisplaySettings;

DegreeMarks::DegreeMarks(qreal size, QGraphicsItem *p) :
    QGraphicsItem(p), _size(size), _marks(UISettings::DMCountMarks), _labelFontMetrics(_labelFont)
{
    _labelFont.setPixelSize(UISettings::DMFontSize);
    _labelFont.setBold(true);
    _labelFontMetrics = QFontMetricsF(_labelFont);

    update(_shipPos, _displayType);
}

QRectF DegreeMarks::boundingRect() const
{
    return { GeometryMath::centeredRectF(_size) };
}

void DegreeMarks::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setFont(_labelFont);
    painter->setPen({UISettings::DMColor, UISettings::DMWidth});
    painter->setRenderHint(QPainter::Antialiasing);
    foreach (const auto& mark, _marks)
    {
        painter->drawLine(mark.mark);
        QRectF sceneRec = GeometryMath::centeredRectF(930, 950);
        QRectF rec = mapRectToScene(mark.labelRect);
        if (sceneRec.contains(rec))
            painter->drawText(mark.labelRect, mark.label);
    }

    if (_displayType == DisplayCourse::North)
        painter->drawLines({_northMark.leftLine, _northMark.rightLine});
}

void DegreeMarks::setDisplayType(DisplaySettings::DisplayCourse display)
{
    update(_shipPos, display);
}

void DegreeMarks::setShipPos(const QPointF& shipPos)
{
    update(shipPos, _displayType);
}

void DegreeMarks::update(const QPointF& shipPos, DisplaySettings::DisplayCourse display)
{
    if (_shipPos == shipPos && display == _displayType && !_firstInitFlag)
        return;

    _shipPos = shipPos;
    _displayType = display;

    enum NorthPoints
    {
        Left, Midle, Right
    };
    QPointF northMarkPoints[3]; // Точки меток градусов -1 0 1 (слево и справо от нуля)

    // Рассчитываем сразу 2 противоположные метки градусов
    static constexpr int CountIterations = UISettings::DMCountMarks / 2;
    for(int i = 0; i < CountIterations; ++i)
    {
        qreal markLen = i % 10 == 0 ? UISettings::DMLenHightMarks :
                                      i % 5 == 0 ? UISettings::DMLenMediumMarks :
                                                   UISettings::DMLenSmallMarks;

        // Расчет точек линий меток
        qreal angle = i - 90.0;
        QPair<QPointF, QPointF> points;
        GeometryMath::lineCircleIntersections(_shipPos, angle,
                                      {0,0}, _size / 2,
                                      points);

        QPointF endMark1Point = points.second + CoordinateMath::polarToRect(markLen, angle);
        QPointF endMark2Point = points.first + CoordinateMath::polarToRect(markLen, angle + 180);
        QLineF mark1(points.second,  endMark1Point);
        QLineF mark2(points.first, endMark2Point);

        _marks[i].mark = mark1;
        _marks[i + CountIterations].mark = mark2;

        // Сохранение для отрисовки метки севера
        if (i == 0)
            northMarkPoints[Midle] = points.second;
        if (i == 1)
            northMarkPoints[Right] = points.second;
        else if (i == CountIterations - 1)
            northMarkPoints[Left] = points.first;

        // Расчет позиций значений градусов
        if (i % 10 == 0)
        {
            QString label1 = QString::number(i);
            QString label2 = _displayType == DisplaySettings::DisplayCourse::North ? QString::number(i+180) : QString::number(180 - i);

            QRectF rectFont1 = _labelFontMetrics.boundingRect(label1);
            endMark1Point.ry() -= rectFont1.height() * (1 - i / 180.0);
            if (i == 0) endMark1Point.rx() -= rectFont1.width() / 2;
            rectFont1.moveTo(endMark1Point);

            _marks[i].label = label1;
            _marks[i].labelRect = rectFont1;

            QRectF rectFont2 = _labelFontMetrics.boundingRect(label2);
            if (i == 0) endMark2Point.rx() += rectFont2.width() / 2;
            endMark2Point.ry() -= rectFont2.height() * (i / 180.0);
            endMark2Point.rx() -= rectFont2.width();
            rectFont2.moveTo(endMark2Point);

            _marks[i + CountIterations].label = label2;
            _marks[i + CountIterations].labelRect = rectFont2;
        }
    }

    // Расчет метки севера
    northMarkPoints[Midle] .ry() += UISettings::DMLenNorthMarks;
    _northMark.leftLine = QLineF { northMarkPoints[Left], northMarkPoints[Midle] };
    _northMark.rightLine = QLineF { northMarkPoints[Right], northMarkPoints[Midle] };
}

qreal DegreeMarks::markSize()
{
    return UISettings::DMLenHightMarks + _labelFontMetrics.height();
}

