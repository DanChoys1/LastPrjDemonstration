#ifndef HELLOSPIRAL_H
#define HELLOSPIRAL_H

#include <QGraphicsItem>

#include <QElapsedTimer>
#include <QTimer>

/// Отрисовки спирали начального экрана
class HelloSpiral : public QGraphicsObject
{ Q_OBJECT
public:
    HelloSpiral(qreal size, QGraphicsItem* p = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override;

    /// Отрисовать спираль
    void start();

private:
    /// Генерация спирали
    void generateImg();

private slots:
    /// Обновление углового размера отрисованной спирали
    void updateBearing();

private:
    /// Размер спирали
    qreal _size = 0;
    /// Отрисовываемое изображение
    QPixmap _img;

    /// Текущий угол отрисовки
    qreal _currBearing = 0;

    /// Текущий цвет секции спирали
    std::array<QColor,18>::const_iterator _colorIt;

    /// Время прошедшее с послднего обновления углового размер отрисовки спирали
    QElapsedTimer _elTimer;
    /// Таймер обновления углового ражмера отрисовки
    QTimer _updateTimer;

    /// Высота секции спирали
    const qreal _SectorHeight = 0;
};

#endif // HELLOSPIRAL_H
