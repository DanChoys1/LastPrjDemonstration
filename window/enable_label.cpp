#include "enable_label.h"

#include <QVariant>
#include <QStyle>

namespace
{
constexpr char const* StateDesignation { "state" };
}

EnableLabel::EnableLabel(QWidget *parent) :
    QLabel(parent)
{}

EnableLabel::EnableLabel(QString text, bool isEnable, QWidget *parent) :
    EnableLabel(parent)
{
    setText(text);
    setEnable(isEnable);
}

void EnableLabel::setEnable(bool isEnable)
{
    setProperty(StateDesignation, isEnable);

    // Для обновления отображения
    style()->unpolish(this);
    style()->polish(this);
}
