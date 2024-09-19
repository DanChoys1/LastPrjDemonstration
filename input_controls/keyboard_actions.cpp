#include "keyboard_actions.h"

#include <QEvent>
#include <QKeyEvent>
#include <QCoreApplication>

KeyboardActions::KeyboardActions(QObject *parent) : QObject(parent)
{
    QCoreApplication::instance()->installEventFilter(this);
    _delayTimer.setSingleShot(true);
}

void KeyboardActions::subscribe(QMap<Qt::Key, std::function<void ()>> actions)
{
    auto it = actions.cbegin();
    while(it != actions.cend())
    {
        _actions[(it++).key()] = it.value();
    }
}

void KeyboardActions::setDelay(qreal delay)
{
    _delayTimer.setInterval(delay);
}

bool KeyboardActions::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Type::KeyPress)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() != _prevKey || !_delayTimer.isActive())
        {
            _prevKey = ke->key();
            _delayTimer.start();

            auto action = _actions.find((Qt::Key)ke->key());
            if (action != _actions.end())
                action.value()();
        }
    }

    return QObject::eventFilter(watched, event);
}
