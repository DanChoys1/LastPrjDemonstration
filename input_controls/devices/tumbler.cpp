#include "tumbler.h"

Tumbler::Tumbler(QWidget *parent) :
    ImageWidget(parent)
{}

bool Tumbler::isOn() const
{
    return this->state();
}

void Tumbler::addImage(const QString &path)
{
    ImageWidget::addImage(path);
    this->setCountStates(_CountStates);
}

void Tumbler::addImage(QPixmap img)
{
    ImageWidget::addImage(img);
    this->setCountStates(_CountStates);
}

void Tumbler::mouseReleaseEvent(QMouseEvent *event)
{
    this->setState(!this->state());

    ImageWidget::mouseReleaseEvent(event);

    if (isOn())
    {
        emit on();
        emit changeState(true);
    }
    else
    {
        emit off();
        emit changeState(false);
    }
}
