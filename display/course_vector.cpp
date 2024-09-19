#include "course_vector.h"

#include <QPainter>

#include "geometry_math.h"
#include "coordinate_math.h"
#include "uisettings.h"

CourseVector::CourseVector(qreal size, QGraphicsItem *p) :
    QGraphicsItem(p), _size(size)
{
    update(0,0,0);
}

QRectF CourseVector::boundingRect() const
{
    return { GeometryMath::centeredRectF(_size) };
}

void CourseVector::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen({ UISettings::CVShipDirectionColor, UISettings::CVShipDirectionWidth });
    painter->drawLine(_lineVector);

    if(_isMoveVecVisible)
        painter->drawLines({ _speedArrow.leftLine, _speedArrow.rightLine });
}

qreal CourseVector::size() const
{
    return _size;
}

void CourseVector::setSize(qreal size)
{
    _size = size;
}

qreal CourseVector::course() const
{
    return _course;
}

qreal CourseVector::speed() const
{
    return _speed;
}

qreal CourseVector::moveVecTime() const
{
    return _moveVecTime;
}

RectPoint CourseVector::moveVec() const
{
    return CoordinateMath::polarToSceneY(_speed * _moveVecTime, _course);
}

void CourseVector::update(qreal course, qreal speed, qreal moveVecTime)
{
    _course = course;
    _speed = speed;
    _moveVecTime = moveVecTime;

    qreal maxDist = CoordinateMath::distance(_size / 2, _size / 2);
    QPointF coursePoint = CoordinateMath::polarToRectY(maxDist, _course);
    _lineVector.setPoints(QPointF{0,0}, CoordinateMath::toScene(coursePoint));

    QPointF midlPoint = CoordinateMath::polarToSceneY(_speed * _moveVecTime, _course);
    _speedArrow.leftLine  = { midlPoint, midlPoint + CoordinateMath::polarToSceneY(UISettings::CVArrowLen, _course + 180 - UISettings::CVArrowAngle) };
    _speedArrow.rightLine = { midlPoint, midlPoint + CoordinateMath::polarToSceneY(UISettings::CVArrowLen, _course + 180 + UISettings::CVArrowAngle) };
}

void CourseVector::updateSpeed(qreal speed)
{
    update(_course, speed, _moveVecTime);
}

void CourseVector::updateCourse(qreal course)
{
    update(course, _speed, _moveVecTime);
}

void CourseVector::updateMoveVecTime(qreal time)
{
    update(_course, _speed, time);
}

void CourseVector::setMovVecVisible(bool isVisible)
{
    _isMoveVecVisible = isVisible;
}
