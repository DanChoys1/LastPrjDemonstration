#ifndef COORDINATEMATH_H
#define COORDINATEMATH_H

#include "polar_point.h"
#include "geo_point.h"
#include "rect_point.h"

#include <QPair>
#include <QRectF>

namespace CoordinateMath
{
    /// Полярные координаты (угол считается от оси X против часовой) в прямоугольные
    RectPoint polarToRect(qreal dist, qreal bear);
    /// Полярные координаты (угол считается от оси X против часовой) в прямоугольные
    RectPoint polarToRect(const PolarPoint &p);

    /// Полярные координаты (угол считается от оси Y по часовой) в прямоугольные
    RectPoint polarToRectY(qreal dist, qreal bear);
    /// Полярные координаты (угол считается от оси Y по часовой) в прямоугольные
    RectPoint polarToRectY(const PolarPoint &p);

    /// Полярные координаты (угол считается от оси Y по часовой) в прямоугольные на сцене
    QPointF polarToSceneY(qreal dist, qreal bear);
    /// Полярные координаты (угол считается от оси Y по часовой) в прямоугольные на сцене
    QPointF polarToSceneY(const PolarPoint &p);

    /// Прямоугольные координаты в полярные (угол считается от оси Y по часовой)
    PolarPoint rectToPolarY(qreal x, qreal y);
    /// Прямоугольные координаты в полярные (угол считается от оси Y по часовой)
    PolarPoint rectToPolarY(const RectPoint &pos);

    /// Географические координаты в прямоугольные
    RectPoint geoToRect(qreal lat, qreal lon, qreal centerLat = 0);
    /// Географические координаты в прямоугольные
    RectPoint geoToRect(const GeoPoint &geoP);

    /// Прямоугольные координаты в географические
    GeoPoint rectToGeo(qreal x, qreal y, qreal centerLat = 0);
    /// Прямоугольные координаты в географические
    GeoPoint rectToGeo(const RectPoint &rectP, qreal centerLat = 0);

    /// Полярные координаты в географические
    GeoPoint polarToGeo(qreal dist, qreal bear, qreal centerLat = 0);
    /// Полярные координаты в географические
    GeoPoint polarToGeo(const PolarPoint &polarP, qreal centerLat = 0);

    /// Расчет пленга по координатам
    qreal bearing(qreal x, qreal y);
    /// Расчет пленга по координатам
    qreal bearing(const QPointF &coordinates);

    /// Расчет дистанции по координатам
    qreal distance(qreal x, qreal y);
    /// Расчет дистанции по координатам
    qreal distance(const QPointF &coordinates);
    /// Расчет дистанции по координатам
    qreal distance(const GeoPoint &coordinates);

    /// Перевод координат в координаты сцены
    QPointF toScene(QPointF coord);
    /// Перевод координат в координаты сцены
    QPointF toScene(RectPoint coord);

    /// Перевод из координаты сцены
    QPointF fromScene(QPointF coord);

    /// Перевод угла в (0 - 360)
    qreal to360(qreal angle);
    /// Перевод угла в (-180 - 180)
    qreal to180(qreal angle);

    //////////////////     ПЕРЕСЧЕТ ЕДЕНИЦ ИЗМЕРЕНИЯ      //////////////////

    /// Метров в мили
    static constexpr qreal MetersPerMile = 1852.;
    /// Метров в секунду в узлы
    static constexpr qreal MetSecPerUzl = 0.514;

    /// Переводит морские мили в метры
    constexpr qreal milToMet(qreal miles)
    {
        return miles * MetersPerMile;
    }

    /// Переводит метры в морские мили
    constexpr qreal metToMil(qreal meters)
    {
        return meters / MetersPerMile;
    }

    /// Переводит морские узлы в метры в секунду
    constexpr qreal uzlToMetSec(qreal uzl)
    {
        return uzl * MetSecPerUzl;
    }

    /// Переводит морские метры в секунду в узлы
    constexpr qreal metSecToUzl(qreal metSec)
    {
        return metSec / MetSecPerUzl;
    }

    ////////////////////////////////////////////////////////////////////////
}

#endif // COORDINATEMATH_H
