#ifndef SITUATIONGROUP_H
#define SITUATIONGROUP_H

#include <QGraphicsItemGroup>

/// Группа элементов отображения ситуации.
/// Отрисовывает все элементы только внутри круговой области дисплея
class SituationGroup : public QGraphicsItem
{
public:
    SituationGroup(qreal size, QGraphicsItem* p = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

    /// Круговая область отрисовки элементов стуации
    QPainterPath shape() const override;

    /// Размер области
    qreal size() const;
    /// Размер области
    void setSize(qreal size);

    /// Сместить ситуацию
    void setPos(QPointF pos);
    /// Перемещение к pos с проверкой расстояния до границы,
    /// если расстояние меньше заданного (1/3 радиуса) начинаем отображать с противоположной стороны диаметра.
    /// Возвращает получившуюся позицию относительно центра
    QPointF moveBy(QPointF movePos);
    /// Позиция ситуации
    QPointF pos() const;
    /// Повернуть ситуацию
    void setRotation(qreal a);
    /// Поворот ситуацию
    qreal rotation() const;

    /// Добавление элемента ситуации
    void addToGroup(QGraphicsItem *item);

private:
    /// Размер области отрисовки
    qreal _size = 0;
    /// Прямоугольник области отрисовки
    QRectF _rect;
    /// Круговая область отрисовки элементов стуации
    QPainterPath _shapePath;

    /// Группа для хранения элементов группы
    QGraphicsItemGroup _sitGroup { this };

};

#endif // SITUATIONGROUP_H
