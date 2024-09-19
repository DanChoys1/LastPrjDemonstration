#ifndef TARGETSMANAGER_H
#define TARGETSMANAGER_H

#include "target_data.h"
#include "polar_rect.h"
#include "proton_targets.h"

#include <QQueue>
#include <QSet>
#include <QVector>
#include <ship_data.h>

/// Хранит данные целей с протона.
/// Предоставляет доступ к целям из определенного сектора.
class TargetsManager : public QObject
{ Q_OBJECT
public:
    class IteratorHelper;

    TargetsManager();
    ~TargetsManager();

    /// Цели из сектора
    TargetsManager::IteratorHelper searchInSector(qreal startBear, qreal endBear, qreal stardDist = 0, qreal endDist = std::numeric_limits<qreal>::max());
    /// Цели из сектора
    TargetsManager::IteratorHelper searchInSector(const PolarRect &sector);
    /// Цели из сектора
    TargetsManager::IteratorHelper inSector(const PolarRect &sector);

    /// Захват цели
    void grab(IteratorHelper itTarget);
    /// Сброс цели
    void ungrab(IteratorHelper itTarget);
    /// Выделение цели
    void select(IteratorHelper itTarget);
    /// Снятие выделения с цели
    void unselect();
    /// Выделенная цель
    RlsData::Target selectedTarget();

    /// Цель по id
    RlsData::Target& byId(int id);
    /// Кол-во найденных целей
    int findedTargetCount() const;
    /// Кол-во захваченных целей
    int grabTargetCount() const;

    /// Гео позиция насителя
    TargetsManager::IteratorHelper setSelfShipGeoPos(GeoPoint selfShipGeoPos);
    /// Курс носителя
    void setSelfCourse(qreal course);
    /// Скорость носителя
    void setSelfSpeed(qreal speed);

    /// Задержка удаления следов цели
    void setDotsDelayTime(qreal time);

    /// Опасная ДКС
    void setDangerDKS(qreal dks);
    /// Опасная ВКС
    void setDangerVKS(qreal vks);

private slots:
    /// Цели обновились
    void protonTargetsUpdated();

private:
    /// Минимальный свободный номер цели
    int nextTargetNum();
    /// Освободить номер
    void freeNumber(int num);

private:
    /// Цели с протона. Ключ - пеленг
    QHash<int, RlsData::Target> _findedTargets;
    /// Обновленные цели с протона
    ProtonTargets _protonTargets;
    /// Количество целей которые найдены
    int _findedTargetsCount = 0;

    /// id выделенной цели
    int _selectedTargetId = -1;

    /// Последний используемый номер цели
    int _lastTargetNumber = 0;
    /// Освободившиеся номера цели
    QQueue<int> _freeTargetsNums;

    /// Данные носителя
    RlsData::SelfShip _self;

    /// Время через которое точки цлеей удаляются
    qreal _dotsDelayTime = 0;

    /// Опасная ДКС
    qreal _dangerDKS = 0;
    /// Опасная ВКС
    qreal _dangerVKS = 0;

public:
    /// Итератор для среза массива целей
    class IteratorHelper
    {
    public:
        /// Конструткор
        IteratorHelper(QVector<RlsData::Target*> t);

        void operator ++() const;
        void operator --() const;

        RlsData::Target& operator *();
        const RlsData::Target& operator *() const;
        RlsData::Target* operator ->();
        const RlsData::Target* operator ->() const;

        void toBegin() const;
        bool isEnd() const;

        QVector<RlsData::Target> copy() const;

    private:
         mutable QVector<RlsData::Target*> _targets;
         mutable int _currIndex = 0;

    };
};

#endif // TARGETSMANAGER_H
