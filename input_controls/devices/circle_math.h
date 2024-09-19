#ifndef CIRCLEMATH_H
#define CIRCLEMATH_H

#include <QPointF>

class CircleMath
{
public:
    ///Угол по часовой стрелке от y
    static qreal angleY(qreal x, qreal y);
    ///Угол по часовой стрелке от y
    static qreal angleY(const QPointF& pos);

    ///Преобразует в угол на окружности
    static qreal to360(qreal angle);
    ///Угол между углами
    static qreal minBetweenAngles(qreal angle1, qreal angle2);
    ///Ближайший угол к targetAngle
    static qreal nearestAngle(qreal angle1, qreal angle2, qreal targetAngle);

    ///Угол преращения. Сколько надо добавить к fromAngle чтобы получить toAngle
    static qreal angleIncrement(qreal fromAngle, qreal toAngle);
};

#endif // CIRCLEMATH_H
