#include "proton_targets.h"

#include <QList>
#include <QtMath>

#include <dbhelper_situation/dbhelper_situation.h>

#include "geometry_math.h"

using namespace RlsData;

namespace
{

/// Периуд обновления целей с протона
constexpr qreal UpdateTime = 2e3;

/// Для тестирования
//QList<Target> TestEmulateTargets
//{
////    Target(11 + 00, CoordinateMath::polarToGeo(1000,     0   + 00 ),  10, 45+90),
////    Target(12 + 00, CoordinateMath::polarToGeo(1000,     10  + 00 ),  10, 45+90),
////    Target(13 + 00, CoordinateMath::polarToGeo(1000,     20  + 00 ),  10, 45+90),
////    Target(14 + 00, CoordinateMath::polarToGeo(1000,     30  + 00 ),  10, 45+90),
////    Target(15 + 00, CoordinateMath::polarToGeo(1000,     40  + 00 ),  10, 45+90),
////    Target(16 + 00, CoordinateMath::polarToGeo(1000,     50  + 00 ),  10, 45+90),
////    Target(17 + 00, CoordinateMath::polarToGeo(1000,     60  + 00 ),  10, 45+90),
////    Target(18 + 00, CoordinateMath::polarToGeo(1000,     70  + 00 ),  10, 45+90),
////    Target(19 + 00, CoordinateMath::polarToGeo(1000,     80  + 00 ),  10, 45+90),
////    Target(11 + 10, CoordinateMath::polarToGeo(1000,     0   + 90 ),  10, 45+90),
////    Target(12 + 10, CoordinateMath::polarToGeo(1000,     10  + 90 ),  10, 45+90),
////    Target(13 + 10, CoordinateMath::polarToGeo(1000,     20  + 90 ),  10, 45+90),
////    Target(14 + 10, CoordinateMath::polarToGeo(1000,     30  + 90 ),  10, 45+90),
////    Target(15 + 10, CoordinateMath::polarToGeo(1000,     40  + 90 ),  10, 45+90),
////    Target(16 + 10, CoordinateMath::polarToGeo(1000,     50  + 90 ),  10, 45+90),
////    Target(17 + 10, CoordinateMath::polarToGeo(1000,     60  + 90 ),  10, 45+90),
////    Target(18 + 10, CoordinateMath::polarToGeo(1000,     70  + 90 ),  10, 45+90),
////    Target(19 + 10, CoordinateMath::polarToGeo(1000,     80  + 90 ),  10, 45+90),
////    Target(11 + 30, CoordinateMath::polarToGeo(1000,     0   + 180),  10, 45+90),
////    Target(12 + 30, CoordinateMath::polarToGeo(1000,     10  + 180),  10, 45+90),
////    Target(13 + 30, CoordinateMath::polarToGeo(1000,     20  + 180),  10, 45+90),
////    Target(14 + 30, CoordinateMath::polarToGeo(1000,     30  + 180),  10, 45+90),
////    Target(15 + 30, CoordinateMath::polarToGeo(1000,     40  + 180),  10, 45+90),
////    Target(16 + 30, CoordinateMath::polarToGeo(1000,     50  + 180),  10, 45+90),
////    Target(17 + 30, CoordinateMath::polarToGeo(1000,     60  + 180),  10, 45+90),
////    Target(18 + 30, CoordinateMath::polarToGeo(1000,     70  + 180),  10, 45+90),
////    Target(19 + 30, CoordinateMath::polarToGeo(1000,     80  + 180),  10, 45+90),
////    Target(11 + 20, CoordinateMath::polarToGeo(1000,     0   + 270),  10, 45+90),
////    Target(12 + 20, CoordinateMath::polarToGeo(1000,     10  + 270),  10, 45+90),
////    Target(13 + 20, CoordinateMath::polarToGeo(1000,     20  + 270),  10, 45+90),
////    Target(14 + 20, CoordinateMath::polarToGeo(1000,     30  + 270),  10, 45+90),
////    Target(15 + 20, CoordinateMath::polarToGeo(1000,     40  + 270),  10, 45+90),
////    Target(16 + 20, CoordinateMath::polarToGeo(1000,     50  + 270),  10, 45+90),
////    Target(17 + 20, CoordinateMath::polarToGeo(1000,     60  + 270),  10, 45+90),
////    Target(18 + 20, CoordinateMath::polarToGeo(1000,     70  + 270),  10, 45+90),
////    Target(19 + 20, CoordinateMath::polarToGeo(1000,     80  + 270),  10, 45+90),

//    Target(1, CoordinateMath::polarToGeo(10000 + 10000, 10 ),  10, 45),
//    Target(2, CoordinateMath::polarToGeo(1000 + 10000, 20 ),  10, 45),
//    Target(3, CoordinateMath::polarToGeo(4000 + 10000, 30 ),  10, 45),
//    Target(4, CoordinateMath::polarToGeo(6000 + 10000, 40 ),  10, 45),
//    Target(5, CoordinateMath::polarToGeo(8000 + 10000, 50 ),  10, 45),
//    Target(6, CoordinateMath::polarToGeo(9000 + 10000, 60 ),  10, 45),
//    Target(7, CoordinateMath::polarToGeo(7000 + 10000, 70 ),  10, 45),
//    Target(8, CoordinateMath::polarToGeo(5000 + 10000, 80 ),  10, 45),
//    Target(9, CoordinateMath::polarToGeo(1000 + 10000, 90 ),  10, 45),
//    Target(10, CoordinateMath::polarToGeo(100 + 10000, 180),  10, 45),
//};

///Область обработки целей с протона
static constexpr qreal SearchRect = 40e3;

/// Проверка достаточно ли близко цель к SelfGeoPos чтобы считать ее носителем
bool isSelf(const Target& target, const GeoPoint& selfGeoPos)
{
    return CoordinateMath::distance(GeoPoint{ target.geoPos - selfGeoPos, selfGeoPos.lat }) < 500;
}

/// Высота сканирующего луча
constexpr qreal ScannerHight = 24;
/// Проверка входит ли цель в видимый диапазон радара
bool isBlindArea(const Target& target)
{
    int coeff = target.height < 0 ? -1 : 1;
    qreal maxDist = 4120 * (qSqrt(ScannerHight) + coeff * qSqrt(qAbs(target.height)));
    return target.dist > maxDist;
}

}

ProtonTargets::ProtonTargets(QObject *p) :
    QObject(p)
{
    connect(&_updateTimer, &QTimer::timeout, this, &ProtonTargets::updateTargetData);
    _updateTimer.start(UpdateTime);
    updateTargetData();
}

QHash<int, Target> &ProtonTargets::targets()
{
    return _targets;
}

bool ProtonTargets::contains(int id) const
{
    return _targets.contains(id);
}

Target &ProtonTargets::operator [](int i)
{
    return _targets[i];
}

int ProtonTargets::count() const
{
    return _targets.count();
}

void ProtonTargets::setSelfGeoPos(GeoPoint geoPos)
{
    _selfGeoPos = geoPos;
    qreal coslat = GeometryMath::cos(_selfGeoPos.lat);
    const qreal degDiametrLatitude = SearchRect * 1./1852./60;
    const qreal degDiametrLongitude = degDiametrLatitude / coslat;
    QRectF georect = QRectF(0,0, degDiametrLongitude, degDiametrLatitude);
    georect.moveCenter({_selfGeoPos.lon, _selfGeoPos.lat});
    // Установка фильтра геоквадрата
    DbHelperSituation::instance()->setGeoRect(georect);
}

RlsData::Target ProtonTargets::getTarget(const QVariantList &vals)
{
    enum COLUMNS_SITUATION
    {
        ID,
        EPR_M2,
        LATITUDE_DEG,
        LONGITUDE_DEG,
        HEADING_DEG,
        SPEED_MS,
        ALTITUDE_M
    };

    RlsData::Target target;
    target.geoPos =
    {
        vals.at(COLUMNS_SITUATION::LATITUDE_DEG).toDouble(),
        vals.at(COLUMNS_SITUATION::LONGITUDE_DEG).toDouble()
    };
    // Если это наш корабль
    if (isSelf(target, _selfGeoPos))
        return { -1 };

    target.setRecPos(target.geoPos - _selfGeoPos, _selfGeoPos.lat);
    // Если корабль дальше вдимости радара
    if (isBlindArea(target))
        return { -1 };

    target.id     = vals.at(COLUMNS_SITUATION::ID).toInt();
    target.course = vals.at(COLUMNS_SITUATION::HEADING_DEG).toDouble();
    target.height = vals.at(COLUMNS_SITUATION::ALTITUDE_M).toDouble();
    target.speed  = vals.at(COLUMNS_SITUATION::SPEED_MS).toDouble();
    target.epr    = vals.at(COLUMNS_SITUATION::EPR_M2).toDouble();

    return target;
}

void ProtonTargets::updateTargetData()
{
    QList<QVariantList> situation = DbHelperSituation::instance()->getSituation();
    _targets.clear();
    for (int i = 0; i < situation.count(); ++i)
    {
        RlsData::Target t = getTarget(situation[i]);

        if (t.id > 0)
            _targets.insert(t.id, t);
    }

    emit updated();

//    QList<Target> protonTargets { TestEmulateTargets };
//    foreach(const auto& prTarget, protonTargets)
//    {
//        _targets.insert(prTarget.id, prTarget);
//    }

//    emit updated();

//    for (int i = 0; i < TestEmulateTargets.count(); ++i)
//    {
////        TestEmulateTargets[i].geoPos += GeoPoint(0.02, 0.02);
//    }
}
