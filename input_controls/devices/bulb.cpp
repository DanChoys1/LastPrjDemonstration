#include "bulb.h"

Bulb::Bulb(QWidget *parent) :
    ImageWidget(parent)
{}

bool Bulb::isLight() const
{
    return this->state();
}

void Bulb::setLight(bool isLight)
{
    this->setState(isLight);
    emit lightChanged(this->isLight());
}

void Bulb::addImage(const QString &path)
{
    ImageWidget::addImage(path);
    this->setCountStates(_CountStates);
}

void Bulb::addImage(QPixmap img)
{
    ImageWidget::addImage(img);
    this->setCountStates(_CountStates);
}
