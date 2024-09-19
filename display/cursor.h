#ifndef CURSOR_H
#define CURSOR_H

#include <QGraphicsItem>

#include "coordinate_math.h"

///Курсор удерживает заданную на роидете позицию, но поворот и перемещение относительно сцены
class Cursor : public QGraphicsItem
{
public:
    Cursor(QSize size, QGraphicsItem* p = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

    void update(RectPoint pos);
    void move(RectPoint pos);

    RectPoint pos() const;

    QSizeF size() const;
    void setSize(QSizeF size);

private:
    void updateSize();

private:
    QVector<QLineF> _cursorLines;

    QSizeF _size { 10, 10 };
};

#endif // CURSOR_H
