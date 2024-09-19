#ifndef KEYBOARDCONTROL_H
#define KEYBOARDCONTROL_H

#include "input_control.h"
#include "keyboard_actions.h"

/// Эмулация клавиатуры РЛС для ввода с клавиатуры
class KeyboardControl : public InputControl
{ Q_OBJECT
public:
    KeyboardControl(QWidget* p = nullptr);

private:
    KeyboardActions keyActions;

};

#endif // KEYBOARDCONTROL_H
