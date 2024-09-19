#ifndef RLSMANAGER_H
#define RLSMANAGER_H

#include "rls_window.h"
#include "rls_display.h"
#include "input_control.h"
#include "keyboard_control.h"
#include "targets_manager.h"
#include "menu_container.h"
#include "proton_self_ship.h"
#include "virtual_keyboard.h"
#include "loupe_manager.h"

#include "prj_version_lib/prj_version_dialog.h"

#include <QElapsedTimer>

/// Менеджер управления РЛС
class RlsManager : public QObject
{ Q_OBJECT
public:
    RlsManager();

    /// Окно РЛС
    RlsWindow* rlsWindow();
    /// Окно виртуальной клавиатуры
    VirtualKeyboard* virtualKeyboard();

private:
    /// Начальный прогрев
    void warmingUpStart();
    /// Остановить начальный прогрев
    void warmingUpStop();

    /// Подключение сигналов ввода
    void connectInput(InputControl* inpControl);
    /// Подключение сигналов меню
    void connectMenu();
    /// Подключение обновления носителя
    void connectSelf();

    /// Настройка меню
    void initMenuSettings();

    /// Задать отображаемую дистанцию
    void setShowDistance(qreal dist);

private slots:
    /// Включение питания
    void powerOn(bool isOn);
    /// Таймер прогрева
    void warmingUpTimeUpdate();

    ////////////////////////   Ручной ввод   //////////////////////////////

    /// Увеличение масштаба
    void incScale();
    /// Уменьшение масштаба
    void decScale();

    /// Переключение развертки
    void toogleSearch();

    /// Переключение отображения типа курса
    void toogleDisplayDirectType();
    /// Переключение отображения типа позиции
    void toogleDisplayPosType();
    /// Переключение отображения типа линий движения
    void toggleMoveVectorType();
    /// Задание отображения типа линий движения
    void setMoveVectorType(DisplaySettings::MoveVectorType type);
    /// Переключение отображение колец дальности
    void toggleDistRingsVisible();
    /// Переключение отображения параллельный линий
    void toggleParallelLinesVisible();

    /// Переключение отображение захваченных целей
    void toggleGrabTargetsVisible();

     /// Перемещение пеленга визора
    void addVisorBear(qreal bear);
    /// Перемещение дистанции визора
    void addVisorDist(qreal dist);
    /// Перемещение визора
    void addVisorPos(PolarPoint pos);

    /// Перемещение курсора
    void moveCursor(qreal x, qreal y);
    /// Цель под курсором
    TargetsManager::IteratorHelper getCursorTargets();
    /// Выделение цели
    void selectTarget();
    /// Захват цели
    void grabTarget();
    /// Сброс цели
    void ungrabTarget();

    /// Переключение перемещения центра визора
    void switchTranslateVisor(bool isTranslate);

    /// Переключение режимов отображения точки/следы
    void switchTracesDots();

    /// Открыть меню
    void toggleMenuVisible();
    /// Переключение отображения лупы
    void toogleLoupeClicked();

    ////////////////////////   Ввод через меню   ///////////////////////////

    /// Опасная ДКС
    void setDangerDKS(qreal dks);
    /// Опасная ВКС
    void setDangerVKS(qreal vks);

    /// Время вектора движения
    void setMoveVecTime(qreal time);
    /// Время отображения следов и точек целей
    void setDotsTracesDelayTime(qreal time);

    /// Флаг отображения номеров захваченных целей
    void setDisplayTargetsNum(bool isDisplay);

    /// Начало задания зоны автозахвата
    void settingAutoCatchArea();
    /// Зафиксировать зону автозахвата
    void applyAutoCatchArea();
    /// Скрыть область автозахвата
    void hideAutoCatchArea();

    ////////////////////////   Обновление из БД   //////////////////////////

    /// Обновление целей по пеленгу
    void updateTargets();
    /// Обновление скорости носителя
    void updateSelfSpeed(qreal speed);
    /// Обновление курса носителя
    void updateSelfCourse(qreal course);
    /// Обновление гео позиции носителя
    void updateSelfGeoPos(GeoPoint geoPos);

private:
    ////////////////////////   Настройки   //////////////////////////////

    /// Тип отображения курса
    DisplaySettings::DisplayCourse _displayCourseType;
    /// Тип отображения позиции
    DisplaySettings::DisplayPos _displayPosType;
    /// Тип отображения вектора движения
    DisplaySettings::MoveVectorType _moveVecType;
    /// Тип отображения следов и точек целей
    QVector<DisplaySettings::DisplayTracesDots>::ConstIterator _displayTracesDotsIt;

    /// Флаг задания зоны автозахвата
    bool _isSettingAutoCatchArea = false;

    /// Флаг включения развертки
    bool _isSearchingOn = false;
    /// Скорость вращения луча (гр/мс)
    qreal _rayRotFrequency = 0;

    ////////////////////////   Данные   /////////////////////////////////

    /// Отображаемая дистанция
    QVector<qreal>::ConstIterator _showDists;

    /// Позиция визора
    PolarPoint _visorPos;
    /// Позиция курсора
    RectPoint _cursorPos;

    /// Область автозахвата
    PolarRect _autoCatchArea;

    /// Пеленг поиска целей
    qreal _rayBearing = 0;

    /// Мэнеджер целей
    TargetsManager _targetsManager;

    /// Обновление данных насителея
    ProtonSelfShip _self;

    ///////////////////////////  UI  ////////////////////////////////

    /// Окно РЛС
    RlsWindow _window;
    /// Дисплей развертки
    RlsDisplay _display;
    /// Лупа
    LoupeManager _loupe;

    /////////////////////////////////////////////////////////////////////

    /// Таймер обновления пеленга развертки
    QTimer _updateRayTimer;
    /// Время с прошлого обновления развретки
    QElapsedTimer _elapsRayTimer;

    /// Мэнеджер ввода с клавиатуры
    KeyboardControl _keyboardControl;
    /// Мэнеджер ввода с виртуальной клавиатуры
    VirtualKeyboard _virtualKeyboard;

    /// Контроллер версии приложения
    PrjVersionDialog _prjVers;

    /// Таймер прогрева
    QTimer _warmingUpUpdateTimer;
    /// Время с прошлого обновления прогрева
    QElapsedTimer _warmingUpElapsed;

};

#endif // RLSMANAGER_H
