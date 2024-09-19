#include "button.h"

Button::Button(QWidget *parent) :
    ImageWidget(parent)
{

}

Button::~Button()
{
    _isLight = false;
    _isHighlighting = false;
}

void Button::setHighlighting(bool isHighlighting)
{
    _isHighlighting = isHighlighting;

    if (_isHighlighting)
    {
        this->setCountStates(_CountHighlightingStates);
    }
    else
    {
        this->setCountStates(_CountStates);
    }
}

bool Button::isLight() const
{
    return _isLight;
}

void Button::addImage(const QString &path)
{
    ImageWidget::addImage(path);
    this->setCountStates(_CountStates);
}

void Button::addImage(QPixmap img)
{
    ImageWidget::addImage(img);
    this->setCountStates(_CountStates);
}

bool Button::isPressed() const
{
    return _isPressed;
}

void Button::setLight(bool isLight)
{
    if (_isLight == isLight) return;

    _isLight  = isLight;

    int changeLight = _isLight ? 2 : -2;
    this->setState(this->state() + changeLight);

    emit light(_isLight);
}

void Button::mousePressEvent(QMouseEvent*)
{
    if (!_isPressed)
    {
        this->setState(this->state() + 1);

        _isPressed = true;
        emit pressed();
    }
}

void Button::mouseReleaseEvent(QMouseEvent*)
{
    if (!_isPressed)
    {
        return;
    }

    if (_isHighlighting)
    {
        setLight(!_isLight);
    }

    if (0 <= this->state() - 1)
    {
        this->setState(this->state() - 1);
    }

    _isPressed = false;
    emit released();
}
