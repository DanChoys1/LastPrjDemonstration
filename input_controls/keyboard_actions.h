#ifndef KEYBOARD_ACTIONS_H
#define KEYBOARD_ACTIONS_H

#include <QObject>
#include <QMap>
#include <functional>
#include <QTimer>

/// Эвенты нажатия на клавиатуру
class KeyboardActions : public QObject
{ Q_OBJECT
public:
    explicit KeyboardActions(QObject *parent = nullptr);
    /// Подписаться на клавиши
    void subscribe(QMap<Qt::Key, std::function<void ()> > actions);
    /// Задержка повторения эвента зажатой клавиши
    void setDelay(qreal delay);

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    /// События и подписанные на них кнопки
    QMap<Qt::Key, std::function<void()>> _actions;
    /// Задержка повторения эвента зажатой клавиши
    QTimer _delayTimer;
    /// Предыдущая нажатая клавиша
    int _prevKey = 0;

};

#endif // KEYBOARD_H
