#ifndef RLSDISPLAY_H
#define RLSDISPLAY_H

#include "visor.h"
#include "degree_marks.h"
#include "distance_rings.h"
#include "situation_group.h"
#include "cursor.h"
#include "course_vector.h"
#include "ship_data.h"
#include "targets_display.h"
#include "noise_emulator.h"
#include "auto_catch_area.h"
#include "polar_rect.h"
#include "loupe_display.h"
#include "hello_spiral.h"

#include <QGraphicsScene>
#include <QTimer>

/// Мэнеджер управлениея дисплеем развертки
class RlsDisplay : public QObject
{ Q_OBJECT
public:
    RlsDisplay(qreal size, QObject* p = nullptr);

    /// Графическое представление дисплея
    QGraphicsScene* graphicsScene();

    /// Запустить отображение начальной спирали
    void sayHello();
    /// Скрыть отображение начальной спирали
    void stopSayHello();

    /// Дистанция отображения
    void setDisplayDist(qreal displDist);
    /// количество колец дальности которое отображаем
    void setDistRingsCount(int ringCount);

    /// Отображать ли шум
    void setSearchPower(bool isOn);

    /// Переключить отображение колец дальности
    void toggleDistRingsVisible();
    /// Переключить отображение захваченных целей
    void toggleGrabTargetsVisible();

    /// Задать тип отображения курса
    void setDisplayCourseType(DisplaySettings::DisplayCourse type);
    /// Задать тип отображения позиции
    void setDisplayPosType(DisplaySettings::DisplayPos type);
    /// Задать тип отображения линий движения
    void setMoveVectorType(DisplaySettings::MoveVectorType type);
    /// Задать тип время за которое отображются линии движения
    void setMoveVecTime(qreal time);

    /// Отображать номер захваченных целей
    void setDisplayTargetsNum(bool isDisplay);

    /// Задать скорость носителя
    void setSelfSpeed(qreal speed);
    /// Задать курс носителя
    void setSelfCourse(qreal course);
    /// Задать гео позицию носителя
    void setSelfPos(GeoPoint pos);

    /// Переместить визор
    PolarPoint moveVisor(PolarPoint pos);
    /// Задать позицию визора
    PolarPoint setVisorPos(PolarPoint pos);
    /// Переместить курсор
    RectPoint moveCursor(const RectPoint &pos);

    /// Обновление пеленга и найденных по нему целей
    void updateBearing(qreal bearing, QVector<RlsData::Target> targets);
    /// Захватить цель
    void updateGrabTarget(RlsData::Target target);
    /// Сбросить захват цели
    void resetTargets(const QVector<RlsData::Target> &targets);

    /// Отобразить область автозахвата
    void showAutoCatchArea();
    /// Скрыть область автозахвата
    void hideAutoCatchArea();
    /// Задать позицию области автозахвата
    PolarRect setAutoCatchAreaPos(PolarPoint pos);
    /// Изменить размер облисти автозахвата
    PolarRect incrementAutoCatchAreaSize(PolarPoint pos);

    /// Переключить видимость параллельных линий
    void toggleParallelLinesVisible();
    /// Переместить центр визора
    void translateVisor(const RectPoint &pos);

    /// Задать отображение точек и следов целей
    void setDotsTraces(DisplaySettings::DisplayTracesDots display);

    /// Отобразить лупу по заданному прямоугольнику
    void setLoupeRect(PolarRect rect);
    /// Скрыть лупу
    void hideLoupe();

private:
    /// Обновить поворот ситуции
    void updateSitGroupRotation();
    /// Перерисовать все. С задержкой
    void repaintAll();

    /// Скалирует цели с координатами в метрах в координаты сцены
    void toSceneCoordinates(QVector<RlsData::Target>& targets);
    /// Скалирует цели с координатами в метрах в координаты сцены
    void toSceneCoordinates(RlsData::Target& target);
    /// Скалирует цели
    void scaleTargets(QVector<RlsData::Target>& targets, qreal scale);
    /// Скалирует цели
    void scaleTarget(RlsData::Target& target, qreal scale);

private:
    /// Тип отображения курса
    DisplaySettings::DisplayCourse _displayCourse = DisplaySettings::DisplayCourse::North;
    /// Тип отображения позиция
    DisplaySettings::DisplayPos _displayPos = DisplaySettings::DisplayPos::Relative;

    /// Время за которое отображаются вектора перемещения
    qreal _moveVecTime = 0;

    /// Масштаб отображения (дистанция отображения / пиксель)
    qreal _scale = 1;

    /// Область отрисовки (влкючая метки курса)
    qreal _size;

    /// Позиция носителя
    RectPoint _selfPos;

    //////////////     ГРАФИКА     //////////////

    /// Таймер обновления графики
    QTimer _updateTimer;

    /// Графическое представление дисплея
    QGraphicsScene _grScene;
    /// Метки градусов
    DegreeMarks _degMarks;

    /// Приветственная заставка
    HelloSpiral _helloSpiral;

    //////     ОБСТАНОВКА     //////

    /// Группа элементов обстановки
    SituationGroup _situationGroup;

    /// Кольца дистанции
    DistanceRings _distRings;

    /// Метки пеленга и дистанции
    Visor _visor;
    /// Курсор
    Cursor _cursor;
    /// Курсовой вектор
    CourseVector _courseVec;

    /// Отображение целей
    TargetsDisplay _targetsDisplay;
    /// Отображение зоны автозахвата
    AutoCatchArea _autoCatchArea;
    /// Отображение шума
    NoiseEmulator _noiseEmulator;
    /// Отображение лупы
    LoupeDisplay _loupeDisplay;

    ////////////////////////////////

    /////////////////////////////////////////////

};

#endif // RLSDISPLAY_H
