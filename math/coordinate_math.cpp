#include "coordinate_math.h"

#include "geometry_math.h"
#include <QtMath>

RectPoint CoordinateMath::polarToRect(qreal dist, qreal bear)
{
    return dist * QPointF(GeometryMath::cos(bear), GeometryMath::sin(bear));
}

RectPoint CoordinateMath::polarToRect(const PolarPoint& p)
{
    return polarToRect(p.D, p.B);
}

RectPoint CoordinateMath::polarToRectY(qreal dist, qreal bear)
{
    return dist * QPointF(GeometryMath::sin(bear), GeometryMath::cos(bear));
}

RectPoint CoordinateMath::polarToRectY(const PolarPoint& p)
{
    return polarToRectY(p.D, p.B);
}

QPointF CoordinateMath::polarToSceneY(qreal dist, qreal bear)
{
    return toScene(polarToRectY(dist,bear));
}

QPointF CoordinateMath::polarToSceneY(const PolarPoint& p)
{
    return polarToSceneY(p.D, p.B);
}

PolarPoint CoordinateMath::rectToPolarY(qreal x, qreal y)
{
    return PolarPoint(distance(x, y), bearing(x, y));
}

PolarPoint CoordinateMath::rectToPolarY(const RectPoint& pos)
{
    return rectToPolarY(pos.x, pos.y);
}

RectPoint CoordinateMath::geoToRect(qreal lat, qreal lon, qreal centerLat)
{
    qreal x = milToMet(lon * 60) * GeometryMath::cos(centerLat);
    qreal y = milToMet(lat * 60);

    return RectPoint(x, y);
}

RectPoint CoordinateMath::geoToRect(const GeoPoint& geoP)
{
    return geoToRect(geoP.lat, geoP.lon, geoP.centerLat);
}

GeoPoint CoordinateMath::rectToGeo(qreal x, qreal y, qreal centerLat)
{
    qreal lon = metToMil(x) / 60 / GeometryMath::cos(centerLat);
    qreal lat = metToMil(y) / 60;

    return GeoPoint(lat, lon, centerLat);
}

GeoPoint CoordinateMath::rectToGeo(const RectPoint& rectP, qreal centerLat)
{
    return rectToGeo(rectP.x, rectP.y, centerLat);
}

GeoPoint CoordinateMath::polarToGeo(qreal dist, qreal bear, qreal centerLat)
{
    return rectToGeo(polarToRectY(dist, bear), centerLat);
}

GeoPoint CoordinateMath::polarToGeo(const PolarPoint& polarP, qreal centerLat)
{
    return polarToGeo(polarP.D, polarP.B, centerLat);
}

qreal CoordinateMath::bearing(qreal x, qreal y)
{
    qreal bearing = qRadiansToDegrees(qAtan2(x, y)); //x и y не на местах т.к. считаем aCot

    if (x < 0)
    {
        bearing = 360 + bearing;
    }

    return bearing;
}

qreal CoordinateMath::bearing(const QPointF& coordinates)
{
    return bearing(coordinates.x(), coordinates.y());
}

qreal CoordinateMath::distance(qreal x, qreal y)
{
    return hypot(x, y);
}

qreal CoordinateMath::distance(const GeoPoint &coordinates)
{
    return distance(geoToRect(coordinates));
}

qreal CoordinateMath::distance(const QPointF& coordinates)
{
    return distance(coordinates.x(), coordinates.y());
}

QPointF CoordinateMath::toScene(QPointF coord)
{
    coord.ry() = -coord.y();
    return coord;
}

QPointF CoordinateMath::toScene(RectPoint coord)
{
    coord.y = -coord.y;
    return coord;
}

QPointF CoordinateMath::fromScene(QPointF coord)
{
    return toScene(coord);
}

qreal CoordinateMath::to360(qreal angle)
{
    angle = std::fmod(angle, 360);
    if (angle < 0)
    {
        angle = 360 + angle;
    }

    return angle;
}

qreal CoordinateMath::to180(qreal angle)
{
    qreal bear = to360(angle);

    if (bear > 180)
    {
        bear = - 360 + bear;
    }

    return bear;
}
