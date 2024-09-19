#ifndef DEGREEMARKS_H
#define DEGREEMARKS_H

#include <QFontMetricsF>
#include <QGraphicsItem>

#include "flag.h"
#include "display_setting_enums.h"

/// Отрисовка градусной сетки
class DegreeMarks : public QGraphicsItem
{
    /// Данные градусной метки
    struct Mark
    {
        QLineF mark;
        QString label;
        QRectF labelRect;
    };
    struct NorthMark
    {
        QLineF leftLine;
        QLineF rightLine;
    };

public:
    DegreeMarks(qreal size, QGraphicsItem* p = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

    /// Отображать сетку 0:360 или -180:180
    void setDisplayType(DisplaySettings::DisplayCourse display);
    /// Позиция вокруг которой строится сетка
    void setShipPos(const QPointF& shipPos);

    /// Обновление градусных меток
    void update(const QPointF& shipPos, DisplaySettings::DisplayCourse display);

    ///  Размер области марок
    qreal markSize();

private:

    /// Флаг первой инициализации
    Flag _firstInitFlag { Flag::IsNotChecked };

    /// Область отрисовки
    qreal _size;

    /// Позиция корабля
    QPointF _shipPos;
    /// Тип отоображения (СЕВЕР, НОС)
    DisplaySettings::DisplayCourse _displayType = DisplaySettings::North;

    /// Данные градусных меток
    QVector<Mark> _marks;
    /// Метка севера
    NorthMark _northMark;

    /// Шрифт значений углов
    QFont _labelFont;
    /// Метрика шрифта
    QFontMetricsF _labelFontMetrics;

};

#endif // DEGREEMARKS_H
