#ifndef TARGETSDISPLAY_H
#define TARGETSDISPLAY_H

#include <QGraphicsItem>

#include "target_data.h"
#include "display_setting_enums.h"

/// Отрисовка захваченных целей
class TargetsDisplay : public QGraphicsItem
{
public:
    TargetsDisplay(qreal size, QGraphicsItem* p = nullptr);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    /// Обновить цели
    void update(const QVector<RlsData::Target> &findeTargets);
    /// Обновить цель
    void update(const RlsData::Target &findeTarget);
    /// Сбросить все цели
    void reset();

    /// Отображаемые цели
    QHash<int,RlsData::Target>* targets();

    /// Время за которое отображается вектор движения
    void setMoveVecTime(qreal time);
    /// Тип отображаемого вектора движения
    void setMoveVectorType(DisplaySettings::MoveVectorType type);
    /// Вектор движения носителя
    void setSelfMoveVec(RectPoint moveVec);

    /// Флаг отображения номера целей
    void setDisplayNums(bool isDisplay);

    /// Флаг отображения точек
    void setIsShowDots(bool isShow);

private:
    /// Область отображения целей
    QRectF _boundingRect;
    /// Цели
    QHash<int,RlsData::Target> _targets;

    /// Время за которое отображается вектор движения
    qreal _moveVecTime = 0;
    /// Тип отображаемого вектора движения
    DisplaySettings::MoveVectorType _moveVecType = DisplaySettings::LID;
    /// Вектор движения носителя
    RectPoint _selfMoveVec;

    /// Флаг отображения номера целей
    bool _isShowDots = false;

    /// Флаг отображения точек
    bool _isDisplayNums = false;

};

#endif // TARGETSDISPLAY_H
