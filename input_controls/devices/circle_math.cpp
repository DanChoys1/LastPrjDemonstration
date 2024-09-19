#include "circle_math.h"

#include <QtMath>

qreal CircleMath::angleY(qreal x, qreal y)
{
    qreal angle = -qRadiansToDegrees(qAtan2(x, y)); //x и y не на местах т.к. считаем aCot

    if (x >= 0)
    {
        angle = 360 + angle;
    }

    return angle;
}

qreal CircleMath::angleY(const QPointF &pos)
{
    return angleY(pos.x(), pos.y());
}

qreal CircleMath::to360(qreal angle)
{
    int wholeDivision = static_cast<int>(angle / 360);
    angle = angle - 360 * wholeDivision;

    if (angle < 0)
    {
        angle = 360 + angle;
    }

    return angle;
}

qreal CircleMath::minBetweenAngles(qreal angle1, qreal angle2)
{
    angle1 = to360(angle1);
    angle2 = to360(angle2);

    qreal rightSubtraction = qAbs(angle1 - angle2);
    qreal leftSubtraction = 360 - rightSubtraction;

    return qMin(rightSubtraction, leftSubtraction);
}

qreal CircleMath::nearestAngle(qreal angle1, qreal angle2, qreal targetAngle)
{
    angle1 = to360(angle1);
    angle2 = to360(angle2);
    targetAngle = to360(targetAngle);

    bool isFirsteAngleNearest = minBetweenAngles(targetAngle, angle1) <
                                minBetweenAngles(targetAngle, angle2);

    return isFirsteAngleNearest ? angle1 : angle2;
}

qreal CircleMath::angleIncrement(qreal fromAngle, qreal toAngle)
{
    fromAngle = to360(fromAngle);
    toAngle = to360(toAngle);

    qreal angleBetweenCorners = minBetweenAngles(fromAngle, toAngle);
    qreal furtherAngle = to360(fromAngle + angleBetweenCorners);

    bool isEndAnglefFurther = qFuzzyCompare(furtherAngle, toAngle);
    return  isEndAnglefFurther ? angleBetweenCorners : -angleBetweenCorners;
}
