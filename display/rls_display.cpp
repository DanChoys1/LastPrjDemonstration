#include "rls_display.h"
#include "math.h"
#include "coordinate_math.h"
#include "display_settings.h"
#include "uicolors.h"
#include "uisettings.h"

namespace
{

/// Частота обновления графики
constexpr qreal UpdateDelay { 1e3 / 50 };

}

RlsDisplay::RlsDisplay(qreal size, QObject *p) :
    QObject(p),
    _size(size),
    _grScene(GeometryMath::centeredRectF(_size, _size), this),
    _degMarks(_size),
    _helloSpiral(_size),
    _situationGroup(_size),
    _distRings(_size*2),
    _visor(_size*2),
    _cursor(UISettings::CursorSize),
    _courseVec(_size*2),
    _targetsDisplay(_size),
    _noiseEmulator(_size)
{
    _grScene.setBackgroundBrush(UIColors::MWindowBack);

    _grScene.addItem(&_noiseEmulator);

    _grScene.addItem(&_situationGroup);
    _situationGroup.addToGroup(&_distRings);
    _situationGroup.addToGroup(&_visor);
    _situationGroup.addToGroup(&_courseVec);
    _situationGroup.addToGroup(&_cursor);
    _situationGroup.addToGroup(&_targetsDisplay);
    _situationGroup.addToGroup(&_autoCatchArea);
    _situationGroup.addToGroup(&_loupeDisplay);

//    _grScene.addItem(&_cursor);
    _grScene.addItem(&_helloSpiral);

    _autoCatchArea.hide();

    _grScene.addItem(&_degMarks);

    connect(&_updateTimer, &QTimer::timeout, this, [&]()
    {
        _updateTimer.stop();
        _grScene.update();
    });
    _updateTimer.setInterval(UpdateDelay);
}

QGraphicsScene *RlsDisplay::graphicsScene()
{
    return &_grScene;
}

void RlsDisplay::sayHello()
{
    _helloSpiral.show();
    _helloSpiral.start();
    _situationGroup.hide();
}

void RlsDisplay::stopSayHello()
{
    _helloSpiral.hide();
    _situationGroup.show();
}

void RlsDisplay::setDisplayDist(qreal displDist)
{
    qreal ratioScale = _scale / (displDist / (_size / 2));
    _scale = displDist / (_size / 2);

    _visor.update(_visor.pos() * ratioScale);
    _cursor.update(_cursor.pos() * ratioScale);
    _courseVec.updateSpeed(_courseVec.speed() * ratioScale);
    _noiseEmulator.setCoordinatesScaleRatio(ratioScale);
    _selfPos *= ratioScale;

    PolarRect loupeRect = _loupeDisplay.rect();
    loupeRect.p1.D *= ratioScale;
    loupeRect.p2.D *= ratioScale;
    _loupeDisplay.setRect(loupeRect);

    _targetsDisplay.setSelfMoveVec(_courseVec.moveVec());
    for(auto& target : *_targetsDisplay.targets())
    {
        scaleTarget(target, 1/ratioScale);
    }

    repaintAll();
}

void RlsDisplay::setDistRingsCount(int ringCount)
{
    _distRings.setCount(ringCount);

    repaintAll();
}

void RlsDisplay::setSearchPower(bool isOn)
{
    _noiseEmulator.setSearchPower(isOn);
}

void RlsDisplay::toggleDistRingsVisible()
{
    _distRings.setVisible(!_distRings.isVisible());
}

void RlsDisplay::toggleGrabTargetsVisible()
{
    _targetsDisplay.setVisible(!_targetsDisplay.isVisible());
}

void RlsDisplay::setDisplayCourseType(DisplaySettings::DisplayCourse type)
{
    _displayCourse = type;
    _degMarks.setDisplayType(_displayCourse);
    updateSitGroupRotation();
}

void RlsDisplay::setDisplayPosType(DisplaySettings::DisplayPos type)
{
    _displayPos = type;

    if (_displayPos == DisplaySettings::Relative)
    {
        _situationGroup.setPos({0,0});
        _degMarks.setShipPos({0,0});
        _noiseEmulator.setCenterShift({0,0});
    }

    repaintAll();
}

void RlsDisplay::setMoveVectorType(DisplaySettings::MoveVectorType type)
{
//    _moveVecType = type;
    _targetsDisplay.setMoveVectorType(type);
    _courseVec.setMovVecVisible(type == DisplaySettings::MoveVectorType::LID);
}

void RlsDisplay::setMoveVecTime(qreal time)
{
    _moveVecTime = time;
    _courseVec.updateMoveVecTime(time);
    _targetsDisplay.setMoveVecTime(_moveVecTime);
    _targetsDisplay.setSelfMoveVec(_courseVec.moveVec());

    repaintAll();
}

void RlsDisplay::setDisplayTargetsNum(bool isDisplay)
{
    _targetsDisplay.setDisplayNums(isDisplay);
}

PolarPoint RlsDisplay::moveVisor(PolarPoint pos)
{
    pos.B *= DisplaySettings::VisorSensitiveBearing;
    pos.D *= DisplaySettings::VisorSensitiveDistance;
    _visor.move(pos);

    qreal maxVisorDist = _size/2 * (1 + DisplaySettings::MaxCenterShiftCoeff); // Максимальная дистанция = радиус + максимальная дистанция смещения центра

    if (_visor.pos().D > maxVisorDist)
        _visor.update(PolarPoint { _size/2 * DisplaySettings::MaxCenterShiftCoeff, _visor.pos().B });

    repaintAll();

    return _visor.pos() * _scale;
}

PolarPoint RlsDisplay::setVisorPos(PolarPoint pos)
{
    _visor.update(pos / _scale);

    qreal maxVisorDist = _size/2 * (1 + DisplaySettings::MaxCenterShiftCoeff); // Максимальная дистанция = радиус + максимальная дистанция смещения центра

    if (_visor.pos().D > maxVisorDist)
        _visor.update(PolarPoint { _size/2 * DisplaySettings::MaxCenterShiftCoeff, _visor.pos().B });

    repaintAll();

    return _visor.pos() * _scale;
}

RectPoint RlsDisplay::moveCursor(const RectPoint &pos)
{
    _cursor.move(CoordinateMath::toScene(pos) * DisplaySettings::CursorSensitive);
    PolarPoint cursorScenePos = CoordinateMath::rectToPolarY(_cursor.mapToScene({0,0}));
    if (cursorScenePos.D > _size/2)
    {
        _cursor.move(_cursor.mapFromScene(CoordinateMath::polarToRectY(_size/2, cursorScenePos.B)));
    }

    repaintAll();

    return CoordinateMath::fromScene(_cursor.pos()) * _scale;
}

void RlsDisplay::setSelfSpeed(qreal speed)
{
    _courseVec.updateSpeed(speed / _scale);
    _targetsDisplay.setSelfMoveVec(_courseVec.moveVec());

    repaintAll();
}

void RlsDisplay::setSelfCourse(qreal course)
{
    _courseVec.updateCourse(course);
    _targetsDisplay.setSelfMoveVec(_courseVec.moveVec());

    updateSitGroupRotation();
}

void RlsDisplay::setSelfPos(GeoPoint pos)
{
    QPointF newPos = CoordinateMath::toScene(CoordinateMath::geoToRect(pos)) / _scale;
    if (_displayPos == DisplaySettings::True)
    {
        QPointF sitPos = _situationGroup.moveBy(newPos - _selfPos);
        _degMarks.setShipPos(sitPos);
        _noiseEmulator.setCenterShift(sitPos);
    }

    _selfPos = newPos;

    _noiseEmulator.setSelfPos(_selfPos);

    repaintAll();
}

void RlsDisplay::updateBearing(qreal bearing, QVector<RlsData::Target> targets)
{
    toSceneCoordinates(targets);
    _noiseEmulator.update(bearing, targets);

    repaintAll();
}

void RlsDisplay::updateGrabTarget(RlsData::Target target)
{
    toSceneCoordinates(target);
    _targetsDisplay.update(target);

    repaintAll();
}

void RlsDisplay::resetTargets(const QVector<RlsData::Target>& targets)
{
    _targetsDisplay.reset();
    foreach(const auto& t, targets)
    {
        if (t.isGrab)
        {
            RlsData::Target target = t;
            toSceneCoordinates(target);
            _targetsDisplay.update(target);
        }
    }

    repaintAll();
}

void RlsDisplay::showAutoCatchArea()
{
    _autoCatchArea.show();
}

void RlsDisplay::hideAutoCatchArea()
{
    _autoCatchArea.hide();
}

PolarRect RlsDisplay::setAutoCatchAreaPos(PolarPoint pos)
{
    _autoCatchArea.setDistPos(pos.D / _scale);
    _autoCatchArea.setBearing(pos.B);

    repaintAll();

    return PolarRect {  _autoCatchArea.distPos() * _scale, _autoCatchArea.bearing(),
                _autoCatchArea.maxDistPos() * _scale, _autoCatchArea.maxBearPos() };
}

PolarRect RlsDisplay::incrementAutoCatchAreaSize(PolarPoint pos)
{
    pos.B *= DisplaySettings::VisorSensitiveBearing;
    pos.D *= DisplaySettings::VisorSensitiveDistance;
    _autoCatchArea.incrementSize(pos.D, pos.B);

    repaintAll();

    return PolarRect {  _autoCatchArea.distPos() * _scale, _autoCatchArea.bearing(),
                _autoCatchArea.maxDistPos() * _scale, _autoCatchArea.maxBearPos() };
}

void RlsDisplay::toggleParallelLinesVisible()
{
    _visor.toggleParallelLinesVisible();
}

void RlsDisplay::translateVisor(const RectPoint& pos)
{
    _visor.translate(CoordinateMath::toScene(pos) / _scale);
}

void RlsDisplay::setDotsTraces(DisplaySettings::DisplayTracesDots display)
{
    switch (display)
    {
    case DisplaySettings::DisplayTracesDots::Off:
        _targetsDisplay.setIsShowDots(false);
        _noiseEmulator.setIsShowTraces(false);
        break;
    case DisplaySettings::DisplayTracesDots::Dots:
        _targetsDisplay.setIsShowDots(true);
        _noiseEmulator.setIsShowTraces(false);
        break;
    case DisplaySettings::DisplayTracesDots::Traces:
        _targetsDisplay.setIsShowDots(false);
        _noiseEmulator.setIsShowTraces(true);
        break;
    case DisplaySettings::DisplayTracesDots::TracesDots:
        _targetsDisplay.setIsShowDots(true);
        _noiseEmulator.setIsShowTraces(true);
        break;
    }
}

void RlsDisplay::setLoupeRect(PolarRect rect)
{
    rect.p1.D /= _scale;
    rect.p2.D /= _scale;
    _loupeDisplay.setRect(rect);
    _loupeDisplay.show();
}

void RlsDisplay::hideLoupe()
{
    _loupeDisplay.hide();
}

void RlsDisplay::updateSitGroupRotation()
{
    switch (_displayCourse)
    {
    case DisplaySettings::DisplayCourse::North:
        _situationGroup.setRotation(0);
        _noiseEmulator.setRotation(0);
        break;
    case DisplaySettings::DisplayCourse::Course:
        _situationGroup.setRotation(-_courseVec.course());
        _noiseEmulator.setRotation(-_courseVec.course());
        break;
    }

    repaintAll();
}

void RlsDisplay::repaintAll()
{
    if (!_updateTimer.isActive())
    {
        _updateTimer.start();
    }
}

void RlsDisplay::toSceneCoordinates(QVector<RlsData::Target> &targets)
{
    for(auto& target : targets)
    {
        toSceneCoordinates(target);
    }
}

void RlsDisplay::toSceneCoordinates(RlsData::Target &target)
{
    scaleTarget(target, _scale);
    target.relativeRecPos = CoordinateMath::toScene(target.relativeRecPos);
    for(auto& dot : target.dots)
        dot = CoordinateMath::toScene(dot);
}

void RlsDisplay::scaleTargets(QVector<RlsData::Target> &targets, qreal scale)
{
    for(auto& target : targets)
    {
        scaleTarget(target, scale);
    }
}

void RlsDisplay::scaleTarget(RlsData::Target &target, qreal scale)
{
    target.relativeRecPos /= scale;
    target.speed /= scale;
    for(auto& dot : target.dots)
        dot /= scale;
}
