#ifndef DIAL_H
#define DIAL_H

#include "image_widget.h"

///
/// \brief Прибор кручения
///
class Dial : public ImageWidget
{
    Q_OBJECT

public:
    ///Конструктор
    Dial(QWidget* parent = nullptr);
    ///Деструктор
    ~Dial();

    ///Чувствительность поворота колесом мыши
    void setWheelSensitive(qreal sensitive);
    ///Чувствительность поворота колесом мыши
    qreal wheelSensitive() const;

    ///Слежение нулевой координаты изображения за куросром
    void setTrackingMode(bool isTracking);

    void setRotLimits(qreal min, qreal max);

    qreal maxRot() const;
    qreal minRot() const;

    qreal rotation() const;

public slots:
    ///Задает поворот
    void setRotation(qreal rotate);
    ///Задает изменение поворота
    void setDiffRotation(qreal diffRotate);

signals:
    ///Значение изменения поворота
    void rotationChangedTo(qreal);
    ///Значение нового поворота
    void rotationChanged(qreal);

protected:
    ///Событие нажатия мыши
    void mousePressEvent(QMouseEvent *event) override;
    ///Событие отжатия мыши
    void mouseMoveEvent(QMouseEvent *event) override;
    ///Событие колеса мыши
    void wheelEvent(QWheelEvent *event) override;

private:
    ///Чувствительность колеса мыши
    qreal _wheelSensitive = 1;

    ///Следящий режим. Слежение нулевой координаты изображения за куросром
    bool _isTrackingMode = false;
    ///Позиция мыши
    QPointF _mousePos;

    ///Максимальное значение
    qreal _maxRot = std::numeric_limits<qreal>::max();
    ///Минимальное значение
    qreal _minRot = -_maxRot;
};

#endif // DIAL_H
