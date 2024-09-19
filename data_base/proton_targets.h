#ifndef PROTONTARGETS_H
#define PROTONTARGETS_H

#include <QHash>
#include <QObject>
#include <QTimer>

#include "target_data.h"
#include "rect_point.h"

/// Оповещение об обновлении данных целей
class ProtonTargets : public QObject
{ Q_OBJECT
public:
    ProtonTargets(QObject* p = nullptr);

    /// Данные целей
    QHash<int, RlsData::Target>& targets();
    /// Содержит ли цель с id == id
    bool contains(int id) const;
    /// Цель с id == i
    RlsData::Target& operator [](int i);
    /// Количество целей
    int count() const;

public:
    /// Позиция носителя.
    /// Необходимо задавать для получения целей с БД из определенной области
    void setSelfGeoPos(GeoPoint geoPos);

signals:
    /// Сигнал об изменении данных целей
    void updated();

private:
    /// Десериализация данных прешедших с протона
    RlsData::Target getTarget(const QVariantList &vals);

private slots:
    /// Чтение БД и заполенние массива _targets актуальными данными
    void updateTargetData();

private:
    /// Данные целей с протона
    QHash<int, RlsData::Target> _targets;
    /// Позиция носителя
    GeoPoint _selfGeoPos;

    /// Таймер обновления данных целей с протона
    QTimer _updateTimer;

};

#endif // PROTONTARGETS_H
