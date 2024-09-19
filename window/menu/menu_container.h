#ifndef MENUCONTAINER_H
#define MENUCONTAINER_H

#include "menu_item_button.h"
#include "menu_item_value.h"
#include "menu_widget.h"
#include "onoff_value_widget.h"
#include "value_widget.h"

/// Контейнер Меню
class MenuContainer : public QWidget
{ Q_OBJECT
public:
    MenuContainer(QWidget *parent = nullptr);

    /// Флаг отображения меню
    bool isVisible() const;

    /// Отобразить начальное меню
    void show();
    /// Скрыть меню
    void hide();

    /// Переход на начальное меню
    void toDefaultMenu();

    /// Задать текущее меню
    void setCurrentMenu(MenuWidget* menu);

public slots:
    void next();
    void prev();

    void toogleEdit();

    void toogleVisible();

public:
    // ГЛАВНОЕ МЕНЮ
    MenuItemButton brightSwitch     { QStringLiteral("") };
    MenuItemButton autoWaySwitch    { QStringLiteral("") };
    MenuItemButton installerSwitch  { QStringLiteral("") };
    MenuItemButton waySwitch        { QStringLiteral("") };
    MenuItemButton mapSwitch        { QStringLiteral(""), false };
    MenuItemButton identificSwitch  { QStringLiteral("") };
    MenuItemButton controlSwitch    { QStringLiteral("") };
    MenuItemButton documentSwitch   { QStringLiteral("") };
    MenuItemButton onOffBtnSwitch   { QStringLiteral("") };

    // АВТОПРОКЛАДКА
    MenuItemValue  dangerDKS            { QStringLiteral(""),     new ValueWidget(2, 2, 0.01, 8) };
    MenuItemValue  dangerVKS            { QStringLiteral(""),     new ValueWidget(30, 0, 5, 60) };
    MenuItemValue  moveVecTime          { QStringLiteral(""),        new ValueWidget(10, 0, 5, 60) };
    MenuItemValue  dotsTracesDelayTime  { QStringLiteral(""),      new EnumWidget({{1,"1"}, {2,"2"}, {3,"3"}, {6,"6"}, {12,"12"}}, 6) };
    MenuItemValue  onOffTargetsNum      { QStringLiteral(""),   new OnOffValueWidget(OnOffValueWidget::OnOff) };
    MenuItemButton resetAreaBtn         { QStringLiteral(""),         false };
    MenuItemButton setAreaBtn           { QStringLiteral(""),         true };

    // АИС
    MenuItemButton aisSwitch        { QStringLiteral("") };

private:
    void bindSwitchToMenu(MenuItemButton* btn, MenuWidget* menu);

private:
    MenuWidget* _currMenu = nullptr;
    MenuWidget* _defaultMenu = nullptr;


};

#endif // MENUCONTAINER_H
