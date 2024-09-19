#include "menu_item.h"

#include <QLayout>
#include <QVariant>
#include <QStyle>

namespace
{

constexpr char const* StateDesignation { "state" };

}

MenuItem::MenuItem(QWidget *p) :
    QFrame(p)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 3, 5, 3);
}

MenuItem::~MenuItem()
{}

void MenuItem::setState(State state)
{
    if (!_isEnable && state == _state)
        return;

    State oldState = _state;
    _state = state;

    switch (state)
    {
        case Unselect:
        {
            setProperty(StateDesignation, State::Unselect);

            if (oldState == Edit) emit stopEdit();
            emit unselected();

            break;
        }
        case Select:
        {
            setProperty(StateDesignation, State::Select);

            if (oldState == Unselect) emit selected();
            if (oldState == Edit) emit stopEdit();

            break;
        }
        case Edit:
        {
            setProperty(StateDesignation, State::Edit);

            emit startEdit();

            break;
        }
    }

    repolish();
    update();
}

MenuItem::State MenuItem::state() const
{
    return _state;
}

void MenuItem::unselect()
{
    setState(Unselect);
}

void MenuItem::select()
{
    setState(Select);
}

void MenuItem::edit()
{
    setState(Edit);
}

void MenuItem::repolish()
{
    repolish(this);
}

void MenuItem::repolish(QWidget *self)
{
    // Для обновления отображения
    style()->unpolish(self);
    style()->polish(self);
}

void MenuItem::setEnable(bool isEnable)
{
    _isEnable = isEnable;
    if (!_isEnable)
    {
        unselect();
        emit unenable();
    }
    else
    {
        emit enable();
    }
}

bool MenuItem::isEnable() const
{
    return _isEnable;
}
