#include "targets_manager.h"
#include "geometry_math.h"

#include <QLineF>
#include <cmath>

using namespace RlsData;

namespace
{

/// Кол-во раз которое цель должна быть найдена, после которого будем отображать курс и скорость
constexpr int MoveVectorCalcCount = 3;

/// Расчет ВКС и ДКС
void calcKS(Target& target, const SelfShip& self)
{
    qreal dVx = target.speed*GeometryMath::sin(target.course)-self.speed*GeometryMath::sin(self.course);
    qreal dVy = target.speed*GeometryMath::cos(target.course)-self.speed*GeometryMath::cos(self.course);
    qreal dx  = target.relativeRecPos.x;
    qreal dy  = target.relativeRecPos.y;
    target.vks = -(dx*dVx + dy*dVy)/(dVx*dVx + dVy*dVy); // время кратч. расстояния
    target.dks = CoordinateMath::distance(dx + dVx*target.vks, dy + dVy*target.vks); // min дистанция
}

/// Расчет ВКС и ДКС
void calcPK(Target& target, const SelfShip& self)
{
    qreal dVx = target.speed*GeometryMath::sin(target.course)-self.speed*GeometryMath::sin(self.course);
    qreal dVy = target.speed*GeometryMath::cos(target.course)-self.speed*GeometryMath::cos(self.course);
    qreal dV = std::hypot(dVx, dVy);
    qreal dx  = target.relativeRecPos.x;
    qreal dy  = target.relativeRecPos.y;

    QLineF fTarLine;
    fTarLine.setP1({dx, dy});
    fTarLine.setP2({dx, dy+1});
    fTarLine.setAngle(target.course);
    QLineF selfLine;
    selfLine.setP2({0, 1});
    selfLine.setAngle(self.course);

    QPointF intersectPos;
    if (selfLine.intersect(fTarLine, &intersectPos) == QLineF::IntersectType::NoIntersection)
    {
        target.dpk = 0;
        target.vpk = 0;
    }
    else
    {
        selfLine.setP2(intersectPos);
        int coeff = static_cast<int>(selfLine.angle()) == static_cast<int>(self.course) ? 1 : -1; // Если углы не совпадают, цель уже прошла эту точку
        target.dpk = coeff * selfLine.length();

        fTarLine.setP2(intersectPos);
        coeff = static_cast<int>(fTarLine.angle()) == static_cast<int>(target.course) ? 1 : -1; // Если углы не совпадают, цель уже прошла эту точку
        target.vpk = coeff * fTarLine.length() / dV;
    }
}

}

TargetsManager::TargetsManager()
{
    connect(&_protonTargets, &ProtonTargets::updated, this, &TargetsManager::protonTargetsUpdated);
}

TargetsManager::~TargetsManager()
{}

TargetsManager::IteratorHelper TargetsManager::searchInSector(qreal startBear, qreal endBear, qreal stardDist, qreal endDist)
{
    return searchInSector({stardDist, startBear, endDist, endBear});
}

TargetsManager::IteratorHelper TargetsManager::searchInSector(const PolarRect& sector)
{
    QVector<Target*> sectorTargets;
    for(Target& fTarget : _findedTargets)
    {
        if (!_protonTargets.contains(fTarget.id))
        {
            // Если цель пропала на протоне, но отображается на РЛС, ставим признак потерянная
            if (sector.contains(fTarget.dist, fTarget.bear) && fTarget.isFinde)
            {
                fTarget.isLost = true;
                sectorTargets.append(&fTarget);
            }
            continue;
        }

        Target& prTarget = _protonTargets[fTarget.id];
        prTarget.setRecPos(prTarget.geoPos - _self.pos, _self.pos.lat);
        if (sector.contains(prTarget.dist, prTarget.bear))
        {
            fTarget.finde(prTarget); // Сохраняеи параметры движения, ставим признак найдена
            fTarget.setDotsDeleteDelay(_dotsDelayTime);
            // Если цель была найдена меньше {MoveVectorCalcCount} раз параметры ее движения считаем не определенными
            if (!fTarget.isGrab || fTarget.countFinde++ < MoveVectorCalcCount)
            {
                fTarget.course = 0;
                fTarget.speed = 0;
                fTarget.dks = 0;
                fTarget.vks = 0;
                fTarget.dpk = 0;
                fTarget.vpk = 0;
            }
            else
            {
                // Расчет ВКС и ДКС
                calcKS(fTarget, _self);
                // Расчет ВПК и ДПК
                calcPK(fTarget, _self);

                // Проверка на опасную цель
                fTarget.isDanger = (0 < fTarget.dks && fTarget.dks < _dangerDKS) && (0 < fTarget.vks && fTarget.vks < _dangerVKS);
            }

            sectorTargets.append(&fTarget);
        }
    }

    return { sectorTargets };
}

TargetsManager::IteratorHelper TargetsManager::inSector(const PolarRect &sector)
{
    QVector<Target*> sectorTargets;
    for(Target& fTarget : _findedTargets)
    {
        if (sector.contains(fTarget.dist, fTarget.bear))
        {
            sectorTargets.append(&fTarget);
        }
    }

    return { sectorTargets };
}

void TargetsManager::grab(IteratorHelper itTarget)
{
    if (!itTarget->isGrab)
    {
        itTarget->isGrab = true;
        itTarget->num = nextTargetNum();
    }
}

void TargetsManager::ungrab(IteratorHelper itTarget)
{
    if (itTarget->isSelect)
        unselect();

    itTarget->isGrab = false;
    itTarget->countFinde = 0;
    freeNumber(itTarget->num);
}

void TargetsManager::select(IteratorHelper itTarget)
{
    unselect();
    itTarget->isSelect = true;
    _selectedTargetId = itTarget->id;
}

void TargetsManager::unselect()
{
    if (_selectedTargetId > 0)
        _findedTargets[_selectedTargetId].isSelect = false;
    _selectedTargetId = -1;
}

RlsData::Target TargetsManager::selectedTarget()
{
    if (_selectedTargetId < 0)
        return { -1 };
    return _findedTargets[_selectedTargetId];
}

Target &TargetsManager::byId(int id)
{
    assert(_findedTargets.contains(id));
    return _findedTargets[id];
}

int TargetsManager::findedTargetCount() const
{
    return _findedTargetsCount;
}

int TargetsManager::grabTargetCount() const
{
    return std::count_if(_findedTargets.cbegin(), _findedTargets.cend(), [](const Target& t)
    {
        return t.isGrab;
    });
}

TargetsManager::IteratorHelper TargetsManager::setSelfShipGeoPos(GeoPoint selfShipGeoPos)
{
    _protonTargets.setSelfGeoPos(selfShipGeoPos);

    _self.pos = selfShipGeoPos;
    QVector<Target*> sectorTargets;
    for(Target& fTarget : _findedTargets)
    {
        fTarget.setRecPos(fTarget.geoPos - selfShipGeoPos, _self.pos.lat);
        if (fTarget.isFinde)
            sectorTargets.append(&fTarget);
    }

    return { sectorTargets };
}

void TargetsManager::setSelfCourse(qreal course)
{
    _self.course = course;
}

void TargetsManager::setSelfSpeed(qreal speed)
{
    _self.speed = speed;
}

void TargetsManager::setDotsDelayTime(qreal time)
{
    _dotsDelayTime = time;
}

void TargetsManager::setDangerDKS(qreal dks)
{
    _dangerDKS = dks;
}

void TargetsManager::setDangerVKS(qreal vks)
{
    _dangerVKS = vks;
}

void TargetsManager::protonTargetsUpdated()
{
    foreach (const Target& t, _protonTargets.targets())
    {
        if (!_findedTargets.contains(t.id))
            _findedTargets.insert(t.id, t);
    }
}

int TargetsManager::nextTargetNum()
{
    _findedTargetsCount++;
    if (_freeTargetsNums.empty())
        return ++_lastTargetNumber;
    else
        return _freeTargetsNums.dequeue();
}

void TargetsManager::freeNumber(int num)
{
    _findedTargetsCount--;
    _freeTargetsNums.enqueue(num);
}

/////////////////////////  SliceIterator  /////////////////////////
TargetsManager::IteratorHelper::IteratorHelper(QVector<Target*> t) :
    _targets(t)
{}

void TargetsManager::IteratorHelper::operator ++() const
{
    ++_currIndex;
}

void TargetsManager::IteratorHelper::operator --() const
{
    --_currIndex;
}

RlsData::Target& TargetsManager::IteratorHelper::operator *()
{
    return *_targets[_currIndex];
}

const RlsData::Target& TargetsManager::IteratorHelper::operator *() const
{
    return *_targets[_currIndex];
}

Target *TargetsManager::IteratorHelper::operator ->()
{
    return _targets[_currIndex];
}

const Target* TargetsManager::IteratorHelper::operator ->() const
{
    return _targets[_currIndex];
}

void TargetsManager::IteratorHelper::toBegin() const
{
    _currIndex = 0;
}

bool TargetsManager::IteratorHelper::isEnd() const
{
    return _targets.count() <= _currIndex;
}

QVector<Target> TargetsManager::IteratorHelper::copy() const
{
    QVector<Target> copyVec;
    foreach (const Target* target, _targets)
    {
        copyVec.append(*target);
    }

    return copyVec;
}
