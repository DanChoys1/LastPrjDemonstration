#ifndef UISETTINGS_H
#define UISETTINGS_H

#include <QColor>
#include <QPen>

//Дисплей обстановки
namespace UISettings
{
    /// Цвет градусной сетки
    const QColor    DMColor                 { 55, 190, 145 };
    /// Толщина градусной сетки
    constexpr qreal DMWidth                 { 2 };
    /// Количество меток градусов
    constexpr int   DMCountMarks            { 360 };
    /// Длина большой (каждой 10) метки
    constexpr qreal DMLenHightMarks         { 5 };
    /// Длина средней (каждой 5)  метки
    constexpr qreal DMLenMediumMarks        { 3 };
    /// Длина малой метки
    constexpr qreal DMLenSmallMarks         { 1 };
    /// Длина метки севера
    constexpr qreal DMLenNorthMarks         { 10 };
    /// Размер шрифта
    constexpr qreal DMFontSize              { 14 };
    /// Толщина шрифта
    constexpr qreal DMFontWeight            { 10 };

    /// Цвет вектора носителя
    const QColor    CVShipDirectionColor    { DMColor };
    /// Толщина вектора носителя
    constexpr qreal CVShipDirectionWidth    { 2.5 };
    /// Длина линий стрелки вектора носителя
    constexpr qreal CVArrowLen              { 15 };
    /// Угол между линиями стрелки вектора носителя
    constexpr qreal CVArrowAngle            { 30 };

    /// Цвет колец дальности
    const QColor    DistanceRingsColor      { Qt::gray };
    /// Толщина колец дальности
    constexpr qreal DistanceRingsWidth      { 2 };

    /// Цвет визора
    const QColor    VisorColor              { "#91bed0" };
    /// Толщина визора дальности
    constexpr qreal VisorDistWidth          { 3 };
    /// Толщина визора пеленга
    constexpr qreal VisorBearWidth          { 2 };

    /// Цвет курсора
    const QColor    CursorColor             { Qt::white };
    /// Толщина курсора
    constexpr qreal CursorWidth             { 1 };
    /// Расстояние между линиями курсора
    constexpr int   CursorSpace             { 3 };
    /// Размер курсора
    constexpr QSize CursorSize              { 25, 25 };

    /// Цвет выделенной области
    const QColor    SelectAreaColor         { Qt::blue };
    /// Толщина выделенной области
    constexpr qreal SelectAreaWidth         { 2 };

    /// Цвет шума цели
    const QColor    TargetsNoiseColor       { Qt::green };
    /// Цвет следов цели
    const QColor    TargetsNoiseTracesColor { 114, 206, 255 };
    /// Размер шума цели
    constexpr qreal TargetsNoiseSize        { 1 };
    /// Размер увеличения шума цели в зависимости от ракурса
    constexpr qreal TargetsNoiseViewScale   { 2 };

    /// Размер захваченных целей
    constexpr qreal TargetMarkSize          { 10 };
    /// Коэффициент размера креста пропавшей цели
    constexpr qreal TargetCrossCoeff        { 1.3 };
    /// Рамзер отображаемых углов прямоугольника выделенной цели
    constexpr qreal SelectRectCornerSize    { 3 };
    /// Рамзер прямоугольника выделенной цели
    constexpr qreal SelectRectSize          { 20 };
    /// Кисть найденной цели
    const QPen      FindedTargetPen         { QColor { QLatin1String("#ff80fd") }, 1.5, Qt::DashLine };
    /// Кисть пропавшей цели
    const QPen      LostTargetPen           { QColor { Qt::red }, 1.5 };
    /// Кисть опасной цели
    const QPen      DangerTargetPen         { QColor { Qt::red }, 1.5, Qt::DashLine };

    /// Кол-во секторов приветственной спирали
    constexpr int   HSSectorsCount          { 32 };
    /// Кол-во секций в секторе приветственной спирали
    constexpr int   HSSectorPieCount        { 20 };
    /// Ширина секции приветственной спирали
    constexpr qreal HSSectorWide            { 360.0 / HSSectorsCount };
    /// Высота секции приветственной спирали
    constexpr qreal HSSectorHeight          { 360.0 / HSSectorPieCount };
    /// Количество цветов секций приветственной спирали
    constexpr int   HSColorsCount           { HSSectorPieCount - 4 };


}

#endif // UISETTINGS_H
