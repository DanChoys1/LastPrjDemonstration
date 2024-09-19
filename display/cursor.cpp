#include "cursor.h"

#include "geometry_math.h"
#include "uisettings.h"

#include <QPainter>
#include <QDebug>

Cursor::Cursor(QSize size, QGraphicsItem *p) :
    QGraphicsItem(p)
{
    setSize(size);
}

QRectF Cursor::boundingRect() const
{
    return GeometryMath::centeredRectF(_size);
}

void Cursor::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    auto p = parentItem();
    if (p != nullptr)
        painter->rotate(-p->rotation());
    else
        qDebug() << "Cursor do not have parent";

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen({UISettings::CursorColor, UISettings::CursorWidth});
    painter->drawLines(_cursorLines);
}

void Cursor::update(RectPoint pos)
{
    setPos(pos.x, pos.y);
}

void Cursor::move(RectPoint pos)
{
    update(mapFromScene(mapToScene(this->pos()) + pos));
}

RectPoint Cursor::pos() const
{
    return QGraphicsItem::pos();
}

QSizeF Cursor::size() const
{
    return _size;
}

void Cursor::setSize(QSizeF size)
{
    _size = size;
    updateSize();
}

void Cursor::updateSize()
{
    qreal h = _size.height() / 2;
    qreal w = _size.width() / 2;

    _cursorLines =
    {
        QLineF{{0, UISettings::CursorSpace}, {0, h}},
        QLineF{{UISettings::CursorSpace, 0}, {w, 0}},
        QLineF{{-UISettings::CursorSpace, 0}, {-w, 0}},
        QLineF{{0, -UISettings::CursorSpace}, {0, -h}},
    };
}
