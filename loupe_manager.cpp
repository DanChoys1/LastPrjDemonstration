#include "loupe_manager.h"

#include "geometry_math.h"

#include <QPainter>

LoupeManager::LoupeManager(qreal size) :
    _size(size), _grScene(GeometryMath::centeredRectF(_size))
{
    QGraphicsLineItem* hLine = new QGraphicsLineItem(-_size*2,0, _size*2,0, &_cursor);
    hLine->setPen(QPen{Qt::white, 2});
    QGraphicsLineItem* vLine = new QGraphicsLineItem(0,-_size*2, 0,_size*2, &_cursor);
    vLine->setPen(QPen{Qt::white, 2});

    _grScene.addItem(&_map);
    _grScene.addItem(&_cursor);

    _map.setPos(-_size/2, -_size/2);

    _grScene.setBackgroundBrush(Qt::black);
}

QGraphicsScene *LoupeManager::graphicsScene()
{
    return & _grScene;
}

void LoupeManager::setActive(bool isActive)
{
    _isActive = isActive;
    _cursor.setPos(0,0);
}

bool LoupeManager::isActive() const
{
    return _isActive;
}

void LoupeManager::setPolarRect(PolarRect rect)
{
    _rect = rect;
    update();
}

RectPoint LoupeManager::moveCursor(int x, int y)
{
    y = -y;
    _cursor.moveBy(x, y);

    if (qAbs(_cursor.x()) > _size/2)
        _cursor.setX(_size/2 * (x < 0 ? -1 : 1));
    if (qAbs(_cursor.y()) > _size/2)
        _cursor.setY(_size/2 * (y < 0 ? -1 : 1));

    return CoordinateMath::polarToRectY(_rect.center() + cursorRelPolarPos());
}

PolarPoint LoupeManager::cursorRelPolarPos() const
{
    return { (-_cursor.y() / (_size/2)) * (_rect.distSize()/2), (_cursor.x() / (_size/2)) * (_rect.bearSize()/2) };
}

void LoupeManager::setFindedTargets(const QVector<RlsData::Target> &findeTargets)
{
    if (findeTargets.isEmpty())
        return;

    foreach(const auto& t, findeTargets)
    {
        if (t.isLost && _targets.contains(t.id))
        {
            _targets.remove(t.id);
        }
        else
        {
            _targets.insert(t.id, t);
        }
    }

    update();
}

void LoupeManager::update()
{
    QPixmap map(_size, _size);
    QPainter painter(&map);
    painter.fillRect(0, 0, _size, _size, Qt::black);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::green);
    painter.translate(map.rect().center());
    foreach(const auto& t, _targets)
    {
        if (_rect.contains({t.dist, t.bear}))
        {
            qreal dist = t.dist - _rect.center().D;
            qreal bear = t.bear - _rect.center().B;
            QRectF elRect = GeometryMath::centeredRectF(20,10);
            elRect.moveCenter({(bear / (_rect.bearSize()/2)) * (_size/2), -(dist / (_rect.distSize()/2)) * (_size/2)});
            painter.drawEllipse(elRect);
        }
    }

    _map.setPixmap(map);
}
