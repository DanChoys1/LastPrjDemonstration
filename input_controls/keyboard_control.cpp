#include "keyboard_control.h"

#include <QMap>

namespace
{

constexpr qreal VisorDistSens = 1;
constexpr qreal VisorBearSens = 1;
constexpr qreal CursorSens = 1;

}

KeyboardControl::KeyboardControl(QWidget *p) :
    InputControl(p)
{
    #define bindSignal(sig) [this](){ emit sig; }

    keyActions.subscribe({
                             { Qt::Key::Key_Equal,          bindSignal(incScaleClicked()) },
                             { Qt::Key::Key_Minus,          bindSignal(decScaleClicked()) },

                             { Qt::Key::Key_Tab,            bindSignal(toggleDisplayDirectTypeClicked()) },
                             { Qt::Key::Key_CapsLock,       bindSignal(toggleDisplayPosTypeClicked()) },
                             { Qt::Key::Key_Shift,          bindSignal(toggleMoveVectorTypeClicked()) },

                             { Qt::Key::Key_Up,             bindSignal(visorDistTurned(VisorDistSens))  },
                             { Qt::Key::Key_Down,           bindSignal(visorDistTurned(-VisorDistSens)) },
                             { Qt::Key::Key_Right,          bindSignal(visorBearTurned(CursorSens))    },
                             { Qt::Key::Key_Left,           bindSignal(visorBearTurned(-CursorSens))     },

                             { Qt::Key::Key_W,              bindSignal(cursorMoved(0, 1))  },
                             { Qt::Key::Key_D,              bindSignal(cursorMoved(1, 0)) },
                             { Qt::Key::Key_S,              bindSignal(cursorMoved(0, -1)) },
                             { Qt::Key::Key_A,              bindSignal(cursorMoved(-1, 0))  },
                             { Qt::Key::Key_Space,          bindSignal(selectTargetClicked()) },
                             { Qt::Key::Key_Alt,            bindSignal(grabTargetClicked()) },
                             { Qt::Key::Key_Z,              bindSignal(ungrabTargetClicked()) },

                             { Qt::Key::Key_Backspace,      bindSignal(menuClicked()) },
                             { Qt::Key::Key_Return,         bindSignal(startEditClicked()) },
                             { Qt::Key::Key_BracketRight,   bindSignal(prevMenuItemCliked()) },
                             { Qt::Key::Key_Backslash,      bindSignal(nextMenuItemCliked()) },
                         });

}
