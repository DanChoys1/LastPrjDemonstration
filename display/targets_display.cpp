#include "targets_display.h"
#include "geometry_math.h"
#include "uisettings.h"

#include <QPainter>
#include <QVector>

namespace
{
/// Отрисовываемый прямоугольник выделенной цели
const QPainterPath SelectRectPath = []()
{
    const QPolygonF poly
    {{
        { 0, UISettings::SelectRectCornerSize }, { 0, 0 }, { UISettings::SelectRectCornerSize, 0 }
    }};

    QTransform tr;
    tr.rotate(90).translate(0, -UISettings::SelectRectSize);
    QPolygonF poly2 = tr.map(poly);
    tr.rotate(90).translate(0, -UISettings::SelectRectSize);
    QPolygonF poly3 = tr.map(poly);
    tr.rotate(90).translate(0, -UISettings::SelectRectSize);
    QPolygonF poly4 = tr.map(poly);

    QPainterPath p;
    p.addPolygon(poly);
    p.addPolygon(poly2);
    p.addPolygon(poly3);
    p.addPolygon(poly4);
    p.translate(-UISettings::SelectRectSize/2, -UISettings::SelectRectSize/2);

    return p;
}();
}

TargetsDisplay::TargetsDisplay(qreal size, QGraphicsItem *p):
    QGraphicsItem(p), _boundingRect(GeometryMath::centeredRectF(size))
{}

QRectF TargetsDisplay::boundingRect() const
{
    return _boundingRect;
}

void TargetsDisplay::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    foreach(const auto& target, _targets)
    {
        painter->setRenderHint(QPainter::Antialiasing);
        // Смещение до метки от начала координат
        painter->translate(target.relativeRecPos);


        // Стабилизация отображения чтобы не поворачивалсь относительно сцены
        qreal situationTurn = CoordinateMath::rectToPolarY(mapFromScene(0,1) - mapFromScene(0,0)).B; // Угол поворота обстановки
        painter->rotate(-situationTurn); // Стабилизируем номер цели, чтобы она всегда была вертикальной

        // Метка цели
        if (target.isLost)
        {
            painter->setPen(UISettings::LostTargetPen);
            painter->drawRect(GeometryMath::centeredRectF(UISettings::TargetMarkSize));
            QRectF crossRect = GeometryMath::centeredRectF(UISettings::TargetMarkSize * UISettings::TargetCrossCoeff);
            painter->drawLines({
                                   QLineF(crossRect.topLeft(), crossRect.bottomRight()),
                                   QLineF(crossRect.bottomLeft(), crossRect.topRight())
                               });
        }
        else
        {
            painter->setPen(target.isDanger ? UISettings::DangerTargetPen : UISettings::FindedTargetPen);
            painter->drawEllipse(GeometryMath::centeredRectF(UISettings::TargetMarkSize));
        }

        // Номер цели
        if (_isDisplayNums)
            painter->drawText(QPointF(0, -UISettings::SelectRectSize/2), QString::number(target.num));

        // Выделение цели
        if (target.isSelect)
        {
            painter->drawPath(SelectRectPath);
        }

        painter->rotate(situationTurn);

        // Вектора движения
        RectPoint moveVec;
        moveVec = CoordinateMath::polarToSceneY(target.speed * _moveVecTime, target.course); // Вектор истинного движения
        if (_moveVecType == DisplaySettings::MoveVectorType::LOD)
        {
            moveVec -= _selfMoveVec; // Вектор относительного движения
        }

        qreal moveVecAngle = CoordinateMath::rectToPolarY(moveVec).B; // Направление вектора движения (т.к. при ЛОД напрвление движение отличается от курса)
        RectPoint shiftToTargetMark = CoordinateMath::polarToRectY(UISettings::TargetMarkSize/2, moveVecAngle); // Смещение до метки цели от позиции цели
        painter->drawLine(shiftToTargetMark, moveVec + shiftToTargetMark);

        // Точки движения цели
        if (_isShowDots)
        {
            foreach(const auto& dot, target.dots)
            {
                painter->drawPoint(dot);
            }
        }

        // Возвращаем в начало координат
        painter->translate(-target.relativeRecPos);
    }
}

void TargetsDisplay::update(const QVector<RlsData::Target>& findeTargets)
{
    foreach (const auto& findeTarget, findeTargets)
    {
        _targets[findeTarget.id] = findeTarget;
    }
}

void TargetsDisplay::update(const RlsData::Target& findeTarget)
{
    _targets[findeTarget.id] = findeTarget;
}

void TargetsDisplay::reset()
{
    _targets.clear();
}

QHash<int, RlsData::Target> *TargetsDisplay::targets()
{
    return &_targets;
}

void TargetsDisplay::setMoveVecTime(qreal time)
{
    _moveVecTime = time;
}

void TargetsDisplay::setMoveVectorType(DisplaySettings::MoveVectorType type)
{
    _moveVecType = type;
}

void TargetsDisplay::setSelfMoveVec(RectPoint moveVec)
{
    _selfMoveVec = moveVec;
}

void TargetsDisplay::setDisplayNums(bool isDisplay)
{
    _isDisplayNums = isDisplay;
}

void TargetsDisplay::setIsShowDots(bool isShow)
{
    _isShowDots = isShow;
}
