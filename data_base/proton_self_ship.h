#ifndef PROTONSELFSHIP_H
#define PROTONSELFSHIP_H

#include <QObject>
#include <QTimer>

#include "ship_data.h"

/// Оповещает об обновлении данных носителя
class ProtonSelfShip : public QObject
{ Q_OBJECT
public:
    /// Актуальные данных по носителю
    RlsData::SelfShip self;

    ProtonSelfShip(QObject* p = nullptr);

    /// Позиция корабля из БД
    GeoPoint geoPoint();
    /// Скорость из БД
    qreal speed();
    /// Курс из БД
    qreal course();

signals:
    /// Обновление данных скорости
    void speedUpdated(qreal speed);
    /// Обновление данных курса
    void courseUpdated(qreal course);
    /// Обновление данных гео позиции
    void geoPosUpdated(GeoPoint geoPos);

private:
    /// Обновляет данные корабля по таймеру
    void connectTimerUpdatedDB();
    /// Обновляет данные корабля по обновлению в БД
    void connectAutoUpdatedDB();

private:
    /// Таймер обновления данных корабля из БД
    QTimer _updateTimer;

};

#endif // PROTONSELFSHIP_H
