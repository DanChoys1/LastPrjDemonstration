#include "progress_widget.h"

#include "uicolors.h"

namespace
{
constexpr qreal MinValue = 0;
constexpr qreal MaxValue = 100;
constexpr qreal Error = 0.1;

const QString ProgressStyle
{
            "background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
             "stop:0 "      + UIColors::Progress.name() +
             ", stop: %1 "  + UIColors::Progress.name() +
             ", stop: %2 "  + UIColors::FrameBack.name() + ")"
};
}

ProgressWidget::ProgressWidget(QWidget *p) :
    QFrame(p)
{
    setValue(0);
}

void ProgressWidget::setValue(qreal val)
{
    _val = validValue(val);
    setStyleSheet(ProgressStyle.arg(validValue(val-Error)).arg(_val));
}

qreal ProgressWidget::value() const
{
    return _val;
}

qreal ProgressWidget::validValue(qreal val) const
{
    return qMin(qMax(val/100, MinValue), MaxValue);
}
