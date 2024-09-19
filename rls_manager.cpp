#include "rls_manager.h"
#include "coordinate_math.h"
#include "geometry_math.h"

using namespace DisplaySettings;

namespace
{

/// Размер дисплея обстановки
constexpr qreal DisplaySize = 895;

/// Размер дисплея лупы
constexpr qreal LoupeSize = 264;
/// Размер области которую отображает лупа
constexpr PolarRect LoupeRect { {0,0}, CoordinateMath::milToMet(0.4), 20 };

/// Время которое идет прогрев при запуске (мс)
constexpr qreal WarmingUpTime = 5e3;

/// Режимы отображения Следы/Точки
const QVector<DisplaySettings::DisplayTracesDots> DisplayTracesDotsArray
{
    DisplaySettings::DisplayTracesDots::Off,
    DisplaySettings::DisplayTracesDots::Dots,
    DisplaySettings::DisplayTracesDots::Traces,
    DisplaySettings::DisplayTracesDots::TracesDots
};

/// Дистанции отображения дисплея обстановки
constexpr std::array<qreal,8> ShowDists
{
    CoordinateMath::milToMet( 0.25 ),
    CoordinateMath::milToMet( 0.5  ),
    CoordinateMath::milToMet( 1.5  ),
    CoordinateMath::milToMet( 6    ),
    CoordinateMath::milToMet( 12   ),
    CoordinateMath::milToMet( 24   ),
    CoordinateMath::milToMet( 48   ),
    CoordinateMath::milToMet( 96   )
};
/// Индекс дистанции которая отображается при запуске
constexpr int StartShownDistIndex = 4;
/// Дистанция которая отображается при запуске
const QVector<qreal>::ConstIterator StartShownDist = ShowDists.begin() + StartShownDistIndex;

/// Дистанция после которой изменяется количество колец дальности с 5 до 6
constexpr qreal ChangeDistRingsCountDistance { CoordinateMath::milToMet(1) };
/// Количество колец дальности при дальности меньше _CangeDistRingsCountDistance
constexpr qreal DistRingsCountLess1mil { 5 };
/// Количество колец дальности при дальности больше _CangeDistRingsCountDistance
constexpr qreal DistRingsCountGreat1mil { 6 };

constexpr qreal UpdateRayFrequency = 1e3 / 30;
/// Начальная частота вращения луча (об/мин)
constexpr qreal StartRayCountTurnPerMin = 15;
constexpr qreal StartRayRotFrequency = 360 * (StartRayCountTurnPerMin / 6e4);

/// Область захвата курсора (метры)
constexpr qreal CursorCatchAreaSize = 100;

}

RlsManager::RlsManager() :
    _display(DisplaySize), _loupe(LoupeSize), _prjVers(&_window), _showDists(StartShownDist),
    _rayRotFrequency(StartRayRotFrequency), _displayTracesDotsIt(DisplayTracesDotsArray.cbegin())
{
    _window.setDisplay(_display.graphicsScene());

    connectInput(&_keyboardControl);
    connectInput(&_virtualKeyboard);
    connectMenu();
    connectSelf();

    initMenuSettings();

    toggleDistRingsVisible();
    setShowDistance(*_showDists);
    setMoveVectorType(MoveVectorType::LID);

    _window.setLoupe(_loupe.graphicsScene());

    _prjVers.setHotkey(Qt::CTRL | Qt::ALT | Qt::Key_Q);

    _virtualKeyboard.show();

    connect(&_updateRayTimer, &QTimer::timeout, this, &RlsManager::updateTargets);

    #ifdef QT_DEBUG
//        warmingUpStop();
//        _window.powerOn(true);
//        toogleSearch();
    #endif
}

RlsWindow *RlsManager::rlsWindow()
{
    return &_window;
}

VirtualKeyboard *RlsManager::virtualKeyboard()
{
    return &_virtualKeyboard;
}

void RlsManager::warmingUpStart()
{
    connect(&_warmingUpUpdateTimer, &QTimer::timeout, this, &RlsManager::warmingUpTimeUpdate);

    _display.sayHello();
    _window.startWarmingUp();
    _window.warmingUpTime(WarmingUpTime / 1000);

    _warmingUpElapsed.start();
    _warmingUpUpdateTimer.start(900);

    _updateRayTimer.stop();
}

void RlsManager::warmingUpStop()
{
    _warmingUpUpdateTimer.stop();
    _display.stopSayHello();
    _window.stopWarmingUp();

    _updateRayTimer.start(UpdateRayFrequency);
}

void RlsManager::connectInput(InputControl *inpControl)
{
    //////// MENU ////////
    connect(inpControl, &InputControl::menuClicked,          this,          &RlsManager::toggleMenuVisible);
    connect(inpControl, &InputControl::startEditClicked,     &_window.menu, &MenuContainer::toogleEdit);
    connect(inpControl, &InputControl::nextMenuItemCliked,   &_window.menu, &MenuContainer::next);
    connect(inpControl, &InputControl::prevMenuItemCliked,   &_window.menu, &MenuContainer::prev);

    //////// DISPLAY ////////
    connect(inpControl, &InputControl::powerOn,                             this, &RlsManager::powerOn);
    connect(inpControl, &InputControl::toggleDistRingsVisibleClicked,       this, &RlsManager::toggleDistRingsVisible);
    connect(inpControl, &InputControl::toggleParallelLinesVisibleClicked,   this, &RlsManager::toggleParallelLinesVisible);
    connect(inpControl, &InputControl::toggleGrabTargetsVisibleClicked,     this, &RlsManager::toggleGrabTargetsVisible);
    connect(inpControl, &InputControl::toggleDisplayDirectTypeClicked,      this, &RlsManager::toogleDisplayDirectType);
    connect(inpControl, &InputControl::toggleDisplayPosTypeClicked,         this, &RlsManager::toogleDisplayPosType);
    connect(inpControl, &InputControl::toggleMoveVectorTypeClicked,         this, &RlsManager::toggleMoveVectorType);
    connect(inpControl, &InputControl::incScaleClicked,                     this, &RlsManager::incScale);
    connect(inpControl, &InputControl::decScaleClicked,                     this, &RlsManager::decScale);
    connect(inpControl, &InputControl::visorBearTurned,                     this, &RlsManager::addVisorBear);
    connect(inpControl, &InputControl::visorDistTurned,                     this, &RlsManager::addVisorDist);
    connect(inpControl, &InputControl::cursorMoved,                         this, &RlsManager::moveCursor);
    connect(inpControl, &InputControl::selectTargetClicked,                 this, &RlsManager::selectTarget);
    connect(inpControl, &InputControl::grabTargetClicked,                   this, &RlsManager::grabTarget);
    connect(inpControl, &InputControl::ungrabTargetClicked,                 this, &RlsManager::ungrabTarget);
    connect(inpControl, &InputControl::switchTranslateVisor,                this, &RlsManager::switchTranslateVisor);
    connect(inpControl, &InputControl::toogleSearch,                        this, &RlsManager::toogleSearch);
    connect(inpControl, &InputControl::switchTracesDots,                    this, &RlsManager::switchTracesDots);
    connect(inpControl, &InputControl::toogleLoupeClicked,                  this, &RlsManager::toogleLoupeClicked);
}

void RlsManager::connectMenu()
{
    connect(&_window.menu.dangerDKS,            &MenuItemValue::valueChanged,   this, &RlsManager::setDangerDKS);
    connect(&_window.menu.dangerVKS,            &MenuItemValue::valueChanged,   this, &RlsManager::setDangerVKS);
    connect(&_window.menu.moveVecTime,          &MenuItemValue::valueChanged,   this, &RlsManager::setMoveVecTime);
    connect(&_window.menu.dotsTracesDelayTime,  &MenuItemValue::valueChanged,   this, &RlsManager::setDotsTracesDelayTime);
    connect(&_window.menu.setAreaBtn,           &MenuItemButton::startEdit,     this, &RlsManager::settingAutoCatchArea);
    connect(&_window.menu.setAreaBtn,           &MenuItemButton::stopEdit,      this, &RlsManager::applyAutoCatchArea);
    connect(&_window.menu.resetAreaBtn,         &MenuItemButton::startEdit,     this, &RlsManager::hideAutoCatchArea);
    connect(&_window.menu.onOffTargetsNum,      &MenuItemValue::valueChanged,   this, &RlsManager::setDisplayTargetsNum);
}

void RlsManager::connectSelf()
{
    connect(&_self, &ProtonSelfShip::speedUpdated,  this, &RlsManager::updateSelfSpeed);
    updateSelfSpeed(_self.self.speed);
    connect(&_self, &ProtonSelfShip::courseUpdated, this, &RlsManager::updateSelfCourse);
    connect(&_self, &ProtonSelfShip::geoPosUpdated, this, &RlsManager::updateSelfGeoPos);
}

void RlsManager::initMenuSettings()
{
    _window.menu.dangerDKS.setValue(_window.menu.dangerDKS.value());
    _window.menu.dangerVKS.setValue(_window.menu.dangerVKS.value());
    _window.menu.moveVecTime.setValue(_window.menu.moveVecTime.value());
    _window.menu.onOffTargetsNum.setValue(true);
}

void RlsManager::incScale()
{
    if (++_showDists == ShowDists.end())
    {
        --_showDists;
        return;
    }

    setShowDistance(*_showDists);
}

void RlsManager::decScale()
{
    if (_showDists == ShowDists.begin())
        return;

    --_showDists;
    setShowDistance(*_showDists);
}

void RlsManager::toogleSearch()
{
    _isSearchingOn = !_isSearchingOn;
//    if (_isSearchingOn)
//    {
//        _rayAngel = 0;
//        _elapsRayTimer.restart();
//    }
    _display.setSearchPower(_isSearchingOn);
}

void RlsManager::setShowDistance(qreal dist)
{
    int ringCount = dist < ChangeDistRingsCountDistance ? DistRingsCountLess1mil :
                                                          DistRingsCountGreat1mil;
    _display.setDistRingsCount(ringCount*2);
    _display.setDisplayDist(dist);

    _window.setDistRingsCount(ringCount);
    _window.setDisplayDist(dist);

    // Для обновления отображающихся значений курсора на _window
    moveCursor(0,0);
}

void RlsManager::powerOn(bool isOn)
{
    if (isOn)
        warmingUpStart();
    else
        warmingUpStop();

    _window.powerOn(isOn);
}

void RlsManager::warmingUpTimeUpdate()
{
    _window.warmingUpTime((WarmingUpTime - _warmingUpElapsed.elapsed()) / 1000);
    if (_warmingUpElapsed.elapsed() > WarmingUpTime)
        warmingUpStop();
}

void RlsManager::toogleDisplayDirectType()
{
    _displayCourseType = _displayCourseType == DisplayCourse::Course ? DisplayCourse::North :
                                                                       DisplayCourse::Course;
    _display.setDisplayCourseType(_displayCourseType);
    _window.setDisplayCourseType(_displayCourseType);
}

void RlsManager::toogleDisplayPosType()
{
    _displayPosType = _displayPosType == DisplayPos::True ? DisplayPos::Relative :
                                                            DisplayPos::True;

    _display.setDisplayPosType(_displayPosType);
    _window.setDisplayPosType(_displayPosType);
}

void RlsManager::toggleMoveVectorType()
{
    setMoveVectorType(_moveVecType == MoveVectorType::LOD ? MoveVectorType::LID :
                                                            MoveVectorType::LOD);
}

void RlsManager::setMoveVectorType(DisplaySettings::MoveVectorType type)
{
    _moveVecType = type;
    _display.setMoveVectorType(_moveVecType);
    _window.setMoveVecType(_moveVecType);
}

void RlsManager::toggleDistRingsVisible()
{
    _display.toggleDistRingsVisible();
}

void RlsManager::toggleParallelLinesVisible()
{
    _display.toggleParallelLinesVisible();
}

void RlsManager::toggleGrabTargetsVisible()
{
    _display.toggleGrabTargetsVisible();
}

void RlsManager::addVisorBear(qreal bear)
{
    addVisorPos({0, bear});
}

void RlsManager::addVisorDist(qreal dist)
{
    addVisorPos({dist, 0});
}

void RlsManager::addVisorPos(PolarPoint pos)
{
    if (!_isSettingAutoCatchArea)
    {
        _visorPos = _display.moveVisor(pos);
        _window.setVisorPos(_visorPos);
    }
    else
    {
        _autoCatchArea = _display.incrementAutoCatchAreaSize(pos);
    }
}

void RlsManager::moveCursor(qreal x, qreal y)
{
    if (_loupe.isActive())
    {
        _cursorPos = _loupe.moveCursor(x,y);
        _window.setCursorPos(_cursorPos);
        return;
    }

    _cursorPos = _display.moveCursor({x, y});
    _window.setCursorPos(_cursorPos);

    if (_isSettingAutoCatchArea)
    {
        _visorPos = _display.setVisorPos(CoordinateMath::rectToPolarY(_cursorPos));
        _window.setVisorPos(_visorPos);
        _autoCatchArea = _display.setAutoCatchAreaPos(_visorPos);
    }
}

TargetsManager::IteratorHelper RlsManager::getCursorTargets()
{
    PolarPoint cursorPolarPos = CoordinateMath::rectToPolarY(_cursorPos);
    PolarPoint recSize(CursorCatchAreaSize, CoordinateMath::bearing(CursorCatchAreaSize, cursorPolarPos.D));
    PolarRect catchRect(cursorPolarPos - recSize, cursorPolarPos + recSize);
    return _targetsManager.inSector(catchRect);
}

void RlsManager::selectTarget()
{
    TargetsManager::IteratorHelper selTarIt = getCursorTargets();
    if (!selTarIt.isEnd() && selTarIt->isGrab)
    {
        // Снимаем выделение цели
        RlsData::Target prevSelTarIt = _targetsManager.selectedTarget();
        if (prevSelTarIt.id > 0)
        {
            prevSelTarIt.isSelect = false;
            _display.updateGrabTarget(prevSelTarIt);
        }

        // Выделяем новую цель
        _targetsManager.select(selTarIt);
        _display.updateGrabTarget(*selTarIt);
        _window.setTargetData(*selTarIt);
    }
}

void RlsManager::grabTarget()
{
    TargetsManager::IteratorHelper targetIt = getCursorTargets();
    if (!targetIt.isEnd() && !targetIt->isGrab)
    {
        _targetsManager.grab(targetIt);
        _display.updateGrabTarget(*targetIt);
    }
}

void RlsManager::ungrabTarget()
{
    TargetsManager::IteratorHelper targetIt = getCursorTargets();
    if (!targetIt.isEnd() && targetIt->isGrab)
    {
        if (targetIt->isSelect)
            _window.clearTargetData();

        _targetsManager.ungrab(targetIt);
        _display.updateGrabTarget(*targetIt);
    }
}

void RlsManager::switchTranslateVisor(bool isTranslate)
{
    if (isTranslate)
        _display.translateVisor(_cursorPos);
    else
        _display.translateVisor({0,0});
}

void RlsManager::switchTracesDots()
{
    if (++_displayTracesDotsIt == DisplayTracesDotsArray.end())
        _displayTracesDotsIt = DisplayTracesDotsArray.begin();

    DisplayTracesDots display = *_displayTracesDotsIt;
   _window.switchTracesDots(display);
   _display.setDotsTraces(display);
   setDotsTracesDelayTime(display != DisplayTracesDots::Off ? _window.menu.dotsTracesDelayTime.value() :
                                                              0);
}

void RlsManager::toggleMenuVisible()
{
    if (!_loupe.isActive())
        _window.toggleMenuVisible();
}

void RlsManager::toogleLoupeClicked()
{
    if (_window.menu.isVisible())
        return;

    _loupe.setActive(!_loupe.isActive());
    _window.toggleLoupeVisible();

    if (_loupe.isActive())
    {
        PolarRect rect = LoupeRect;
        rect.setCenterPos(CoordinateMath::rectToPolarY(_cursorPos));
        _loupe.setPolarRect(rect);
        _display.setLoupeRect(rect);
    }
    else
    {
        _display.hideLoupe();
        moveCursor(0,0);
    }
}

void RlsManager::setDangerDKS(qreal dks)
{
    dks = CoordinateMath::milToMet(dks);
    _window.setDangerDKS(dks);
    _targetsManager.setDangerDKS(dks);
}

void RlsManager::setDangerVKS(qreal vks)
{
    vks *= 60;
    _window.setDangerVKS(vks);
    _targetsManager.setDangerVKS(vks);
}

void RlsManager::setMoveVecTime(qreal time)
{
    time *= 60;
    _display.setMoveVecTime(time);
    _window.setMoveVecTime(time);
}

void RlsManager::setDotsTracesDelayTime(qreal time)
{
    time *= 60;
    _targetsManager.setDotsDelayTime(time);
    _window.setDotsTracesDelayTime(time);
}

void RlsManager::setDisplayTargetsNum(bool isDisplay)
{
    _display.setDisplayTargetsNum(isDisplay);
}

void RlsManager::settingAutoCatchArea()
{
    _window.menu.resetAreaBtn.setEnable(true);

     _autoCatchArea = _display.setAutoCatchAreaPos(_visorPos);
    _display.showAutoCatchArea();

    _isSettingAutoCatchArea = true;
}

void RlsManager::applyAutoCatchArea()
{
    _isSettingAutoCatchArea = false;
}

void RlsManager::hideAutoCatchArea()
{
    _window.menu.resetAreaBtn.setEnable(false);
    _display.hideAutoCatchArea();
}

void RlsManager::updateTargets()
{
    //                                                   Поворот за прошедшее время при заданной частоте
    qreal newRayAngel = CoordinateMath::to360(_rayBearing + _rayRotFrequency * _elapsRayTimer.restart());

    auto findedTargetsIt = _targetsManager.searchInSector(_rayBearing, newRayAngel);
    while (!findedTargetsIt.isEnd())
    {
        if (_autoCatchArea.contains(findedTargetsIt->dist, findedTargetsIt->bear))
            _targetsManager.grab(findedTargetsIt);

        if (findedTargetsIt->isGrab)
            _display.updateGrabTarget(*findedTargetsIt);

        if (findedTargetsIt->isSelect)
            _window.setTargetData(*findedTargetsIt);

        ++findedTargetsIt;
    }

    QVector<RlsData::Target> findedTargets = findedTargetsIt.copy();
    _loupe.setFindedTargets(findedTargets);
    _display.updateBearing(newRayAngel, std::move(findedTargets));
    _window.setTargetCount(_targetsManager.grabTargetCount());

    _rayBearing = newRayAngel;
}

void RlsManager::updateSelfSpeed(qreal speed)
{
    _window.setSelfSpeed(speed);
    _display.setSelfSpeed(speed);
    _targetsManager.setSelfSpeed(speed);
}

void RlsManager::updateSelfCourse(qreal course)
{
    _window.setSelfCourse(course);
    _display.setSelfCourse(course);
    _targetsManager.setSelfCourse(course);
}

void RlsManager::updateSelfGeoPos(GeoPoint geoPos)
{
    _window.setSelfPos(geoPos);
    _display.setSelfPos(geoPos);

    auto targetsIt =_targetsManager.setSelfShipGeoPos(geoPos);
    _display.resetTargets(targetsIt.copy());

    // Для обновления отображающихся значений курсора на _window
    moveCursor(0,0);
}
