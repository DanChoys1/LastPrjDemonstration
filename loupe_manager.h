#ifndef LOUPE_MANAGER_H
#define LOUPE_MANAGER_H

#include <QGraphicsItem>
#include <QGraphicsScene>

#include "target_data.h"
#include "polar_rect.h"
#include "rect_point.h"

/// Менеджер управления лупой
class LoupeManager
{
public:
    LoupeManager(qreal size);

    QGraphicsScene* graphicsScene();

    /// Активировать
    void setActive(bool isActive);
    /// Флаг активности
    bool isActive() const;

    /// Задать область отображения
    void setPolarRect(PolarRect rect);

    /// Перемещение курсора
    RectPoint moveCursor(int x, int y);

    /// Позиция курсора относительно центра области
    PolarPoint cursorRelPolarPos() const;

    /// Задать найденные цели
    void setFindedTargets(const QVector<RlsData::Target> &findeTargets);

private:
    /// Перерисовать найденные цели
    void update();

private:
    /// Размер лупы
    qreal _size = 0;
    /// Область отображения целей
    PolarRect _rect;

    /// Сцена отображения
    QGraphicsScene _grScene;
    /// Курсор
    QGraphicsItemGroup _cursor;
    /// Цели
    QGraphicsPixmapItem _map;

    /// Флаг активности
    bool _isActive = false;

    /// Найденные цели
    QHash<int,RlsData::Target> _targets;
};

#endif // LOUPE_MANAGER_H
