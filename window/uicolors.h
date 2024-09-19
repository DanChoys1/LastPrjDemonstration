#ifndef UICOLORS_H
#define UICOLORS_H

#include <QColor>

//UI
namespace UIColors
{
    /// Цвет фона приложения
    const QColor MWindowBack            { "#26365b" };

    /// Цвет прогрессбара
    const QColor Progress               { 0, 255, 0 };

    /// Цвет статического текста
    const QColor StaticText             { 114, 206, 255 };
    /// Цвет текста с флагом unenable
    const QColor UnenabledText          { "#acacac" };
    /// Цвет динамического текста
    const QColor DynamicText            { "#FFDB58" };

    /// Цвет надписи ЛОД
    const QColor LodType                { Progress };
    /// Цвет надписи ЛИД
    const QColor LidType                { DynamicText };

    /// Цвет заднего фона фрэймов
    const QColor FrameBack              { Qt::black };
    /// Цвет контура
    const QColor FrameBorder            { 55, 190, 145 };
}

#endif // UICOLORS_H
