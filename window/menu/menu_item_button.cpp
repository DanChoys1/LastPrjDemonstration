#include "menu_item_button.h"

#include <QHBoxLayout>
#include <QStyle>

MenuItemButton::MenuItemButton(QString header, bool isEnable, QWidget* p) :
    MenuItem(p)
{
    layout()->addWidget(&_header);
    _header.setText(header);
    setEnable(isEnable);
}

void MenuItemButton::setEnable(bool isEnable)
{
    MenuItem::setEnable(isEnable);
    _header.setEnable(MenuItem::isEnable());
}

void MenuItemButton::repolish()
{
    MenuItem::repolish(this);
}
