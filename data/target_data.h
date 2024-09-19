#ifndef TARGET_DATA_H
#define TARGET_DATA_H

#include "coordinate_math.h"
#include "auto_deleted_vector.h"
#include "geometry_math.h"

namespace RlsData
{
    /// Структура данных цели
    struct Target
    {
        /// id цели в базе данных
        int id = -1;
        /// Номер цели на РЛС
        int num = -1;

        /// Высота
        qreal height = 0;
        /// Дистанция
        qreal dist = 0;
        /// Пелленг
        qreal bear = 0;
        /// Относительные нашему кораблю координаты
        RectPoint relativeRecPos;
        /// Географические координаты
        GeoPoint geoPos;

        /// Курс
        qreal course = 0;
        /// Скорость
        qreal speed = 0;

        /// Дистанция коротчайшего сближения
        qreal dks = 0;
        /// Время коротчайшего сближения
        qreal vks = 0;
        /// Дистанция пересечения курсов
        qreal dpk = 0;
        /// Время пересечения курсов
        qreal vpk = 0;

        /// ЭПР
        qreal epr = 0;
        /// Коэффициент ракурса (на сколько цель повернута к нам бортом) [0,1]
        qreal viewCoeff = 0;

        /// Точки движения цели (координаты относительно цели), удаляемые с заданной задержкой
        AutoDeletedVector<RectPoint> dots;

        /// Цель найдена
        bool isFinde = false;
        /// Цель пропала (для найденных целей, которые мы еще ожидаем найти)
        bool isLost = false;
        /// Признак захвата цели
        bool isGrab = false;
        /// Признак выделения цели
        bool isSelect = false;
        /// Признак опасной цели
        bool isDanger = false;

        /// Количество раз, которое нашли цель
        int countFinde = 0;

        /// Задает относительную позицию корабля
        /// Инициализирует relativeRecPos, dist, bear
        void setRecPos(GeoPoint relativeGeoPos, qreal centerLat = 0)
        {
            relativeGeoPos.centerLat = centerLat;
            setRecPos(CoordinateMath::geoToRect(relativeGeoPos));
        }

        /// Задает относительную позицию корабля.
        /// Инициализирует relativeRecPos, dist, bear, view
        void setRecPos(RectPoint pos)
        {
            relativeRecPos = pos;

            dist = CoordinateMath::distance(relativeRecPos.x, relativeRecPos.y);
            bear = CoordinateMath::bearing(relativeRecPos.x, relativeRecPos.y);

            qreal view = qAbs(GeometryMath::to180(course - bear));
            view = view > 90 ? 180 - view : view;
            viewCoeff = view / 90;
        }

        /// Обновляет данные цели,
        /// сохраняет предыдущее положения в вектор dot
        /// присваивает isFinde = true; isLost = false;
        void finde(const Target& t)
        {
            id      = t.id;
            course  = t.course;
            height  = t.height;
            speed   = t.speed;
            dist    = t.dist;
            bear    = t.bear;
            geoPos  = t.geoPos;

            moveDots(relativeRecPos - t.relativeRecPos); // Обнвление относительного положения точек
            dots.append({0,0});
            relativeRecPos  = t.relativeRecPos;

            epr     = t.epr;
            viewCoeff    = t.viewCoeff;

            isFinde = true;
            isLost = false;
        }

        /// Задержка удаления точек позиций цели
        void setDotsDeleteDelay(qreal delay)
        {
            dots.setDeleteDelay(delay);
        }

        /// Смещение координат отоносительного положения точек
        void moveDots(const RectPoint& moveOn)
        {
            for (auto& dot : dots)
            {
                dot += moveOn;
            }
        }

        Target(int id = -1) :
            id(id)
        {}

        Target(int id, GeoPoint geoPoint, qreal speed = 0, qreal course = 0) :
            id(id), geoPos(geoPoint)
        {
            this->speed = speed;
            this->course = course;
        }

        bool operator ==(const Target& other)
        {
            return id == other.id;
        }

    };
}

#endif // TARGET_DATA_H
