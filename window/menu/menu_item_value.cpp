#include "menu_item_value.h"

#include <QHBoxLayout>
#include <QtDebug>
#include <QStyle>

MenuItemValue::MenuItemValue(QString header, ValueDynamicLabel *value, bool isEnable, QWidget *p) :
    MenuItem(p), _value(value)
{
    layout()->addWidget(&_header);
    _header.setText(header);
    setEnable(isEnable);

    layout()->addWidget(_value);
    _value->setAlignment(Qt::AlignRight);
    connect(_value, &ValueDynamicLabel::valueChanged, this, &MenuItemValue::valueChanged);
}

void MenuItemValue::setEnable(bool isEnable)
{
    MenuItem::setEnable(isEnable);
    _header.setEnable(MenuItem::isEnable());
}

void MenuItemValue::incr()
{
    if (state() == Edit) _value->incr();
}

void MenuItemValue::decr()
{
    if (state() == Edit) _value->decr();
}

void MenuItemValue::setValue(qreal val)
{
    _value->setValue(val);
}

qreal MenuItemValue::value() const
{
    return _value->value();
}

void MenuItemValue::repolish()
{
     MenuItem::repolish(this);
}
