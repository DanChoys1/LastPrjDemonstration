#include "dial.h"

#include <QMouseEvent>

#include "circle_math.h"

Dial::Dial(QWidget *parent) :
    ImageWidget(parent)
{}

Dial::~Dial()
{
    _wheelSensitive = 0;
    _isTrackingMode = false;
    _mousePos = QPointF();
}

void Dial::setWheelSensitive(qreal sensitive)
{
    _wheelSensitive = qAbs(sensitive);
}

qreal Dial::wheelSensitive() const
{
    return _wheelSensitive;
}

void Dial::setTrackingMode(bool isTracking)
{
    _isTrackingMode = isTracking;
}

void Dial::setRotLimits(qreal min, qreal max)
{
    _minRot = min;
    _maxRot = max;
}

qreal Dial::maxRot() const
{
    return _maxRot;
}

qreal Dial::minRot() const
{
    return _minRot;
}

qreal Dial::rotation() const
{
    return this->imageRotation();
}

void Dial::setRotation(qreal rotate)
{
    if (rotate < _minRot)
    {
        rotate = _minRot;
    }
    else if (rotate > _maxRot)
    {
        rotate = _maxRot;
    }

    qreal oldRotation = this->imageRotation();
    ImageWidget::setImageRotation(CircleMath::to360(rotate));

    emit rotationChangedTo(CircleMath::angleIncrement(oldRotation, this->imageRotation()));
    emit rotationChanged(rotate);
}

void Dial::setDiffRotation(qreal diffRotate)
{
    setRotation(this->imageRotation() + diffRotate);
}

void Dial::mousePressEvent(QMouseEvent *event)
{
    _mousePos = event->localPos() - this->rect().center();

    ImageWidget::mousePressEvent(event);
}

void Dial::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::MouseButton::LeftButton)
    {
        QPointF positionFromCenter = event->localPos() - this->rect().center();
        qreal newRotation = CircleMath::angleY(positionFromCenter);

        if (_isTrackingMode)
        {
            setRotation(newRotation);
        }
        else
        {
            newRotation -= CircleMath::angleY(_mousePos);
            _mousePos = event->localPos() - this->rect().center();

            setDiffRotation(newRotation);
        }
    }

    ImageWidget::mouseMoveEvent(event);
}

void Dial::wheelEvent(QWheelEvent *event)
{
    qreal diffRottation = event->angleDelta().y() > 0 ? _wheelSensitive : -_wheelSensitive;
    this->setRotation(this->imageRotation() + diffRottation);

    ImageWidget::wheelEvent(event);
}
