#ifndef MENU_ITEM_BUTTON_H
#define MENU_ITEM_BUTTON_H

#include "menu_item.h"
#include "enable_label.h"

class MenuItemButton :  public MenuItem
{ Q_OBJECT
public:
    MenuItemButton(QString header, bool isEnable = true, QWidget* p = nullptr);

    void setEnable(bool isEnable) override;

    void repolish() override;

private:
    EnableLabel _header;

};

#endif // MENU_ITEM_BUTTON_H
