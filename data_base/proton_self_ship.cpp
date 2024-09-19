#include "proton_self_ship.h"

#include "datamodel/datamodellocal.h"
#include "datamodel/datahelpers.h"

#include "self_ship.h"

namespace
{

constexpr qreal UpdateTime = 1e3;

}

ProtonSelfShip::ProtonSelfShip(QObject *p) :
    QObject(p)
{
//    connect(&_updateTimer, &QTimer::timeout, this, &ProtonSelfShip::connectTimerUpdatedDB);
//    _updateTimer.start(UpdateTime);

//    connectTimerUpdatedDB();

    connectAutoUpdatedDB();
}

GeoPoint ProtonSelfShip::geoPoint()
{
    QStringList list = DATA_VAL(DataHelpers::degpos).toString().split(',');
    qreal lon = list.at(0).toDouble();
    qreal lat = list.at(1).toDouble();

    return GeoPoint(lat, lon, lat);
}

qreal ProtonSelfShip::speed()
{
    return DATA_VAL(DataHelpers::speed).toDouble();
}

qreal ProtonSelfShip::course()
{
    return DATA_VAL(DataHelpers::head).toDouble();
}

void ProtonSelfShip::connectTimerUpdatedDB()
{
    self.pos = geoPoint();
    self.course = course();
    self.speed = speed();

    emit courseUpdated(self.course);
    emit speedUpdated(self.speed);
    emit geoPosUpdated(self.pos);
}

void ProtonSelfShip::connectAutoUpdatedDB()
{
    //Получаем инстанс DataModelLocal
    auto dm = DataModelLocal::instance();
    // Изменение позиции
    dm->getDataObject(DataHelpers::degpos)->addUpdaterFunc(
    [&](const QVariant& val, const QString&)
    {
        QStringList list = val.toString().split(',');
        if (list.count() == 2)
        {
            self.pos.lon  = list.at(0).toDouble();
            self.pos.lat   = list.at(1).toDouble();
            self.pos.centerLat = self.pos.lat;

            emit geoPosUpdated(self.pos);
        }
    });

    // Изменение курса
    dm->getDataObject(DataHelpers::head)->addUpdaterFunc(
    [&](const QVariant& val, const QString&)
    {
        self.course = val.toDouble();
        emit courseUpdated(self.course);
    });

    // Изменение курса
    dm->getDataObject(DataHelpers::speed)->addUpdaterFunc(
    [&](const QVariant& val, const QString&)
    {
        self.speed = val.toDouble();
        emit speedUpdated(self.speed);
    });
}
