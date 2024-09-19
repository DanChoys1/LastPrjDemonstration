#ifndef DISPLAYSETTINGENUMS_H
#define DISPLAYSETTINGENUMS_H


namespace DisplaySettings
{
/// Тип отоображения курс
enum DisplayCourse
{
    North,      /// Север
    Course      /// Курс
};

/// Тип отображения позиции
enum DisplayPos
{
    Relative,   /// Относительное
    True        /// Истиное
};

/// Тип отображения векотора движения
enum MoveVectorType
{
    LID,        /// ЛИД - линии истинного движения
    LOD         /// ЛОД - линии относительного движения
};

/// Отображение точек и следов
enum DisplayTracesDots
{
    Off,        /// Не отображать
    Traces,     /// Следы
    Dots,       /// Точки
    TracesDots  /// Следы и Точки
};

}

#endif // DISPLAYSETTINGENUMS_H
