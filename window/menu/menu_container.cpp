#include "menu_container.h"
#include "onoff_value_widget.h"
#include "value_widget.h"

#include <QVBoxLayout>
#include <QSpacerItem>

MenuContainer::MenuContainer(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
//    layout->insertSpacerItem(0, new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Expanding));
    layout->setContentsMargins(0,0,0,0);

    _defaultMenu = new MenuWidget("",
    {
        &brightSwitch   ,
        &autoWaySwitch  ,
        &installerSwitch,
        &waySwitch      ,
        &mapSwitch      ,
        &identificSwitch,
        &controlSwitch  ,
        &documentSwitch ,
        &onOffBtnSwitch ,
    }, true);
    layout->insertWidget(0, _defaultMenu);
    _currMenu = _defaultMenu;

    MenuWidget* brightness = new MenuWidget("",
    {
        new MenuItemButton(""          ),
        new MenuItemButton(""  ),
        new MenuItemButton(""                ),
        new MenuItemButton(""),
        new MenuItemButton(""             ),
        new MenuItemButton(""            ),
        new MenuItemButton(""             ),
        new MenuItemButton(""           ),
        new MenuItemButton(""             ),
        new MenuItemValue ("",        new EnumWidget({"", ""})),
    });
    layout->insertWidget(0, brightness);
    bindSwitchToMenu(&brightSwitch, brightness);

    MenuWidget* identification = new MenuWidget("",
    {
        new MenuItemValue("",           new EnumWidget({"", "", ""})),
        new MenuItemValue("",            new EnumWidget({"", "", ""}, 1)),
        new MenuItemValue("",  new ValueWidget(4, 1, 4, 180)),
    });
    layout->insertWidget(0, identification);
    bindSwitchToMenu(&identificSwitch, identification);

    MenuWidget* autoWay = new MenuWidget("",
    {
        &dangerDKS,
        &dangerVKS,
        &moveVecTime,
        &dotsTracesDelayTime,
        new MenuItemValue("", new ValueWidget(1, 0, 1, 99), false),
        &onOffTargetsNum,
        &resetAreaBtn,
        &setAreaBtn,
        new MenuItemValue("",                new EnumWidget({"", ""})),
        &aisSwitch,
        new MenuItemValue("",           new OnOffValueWidget(OnOffValueWidget::OnOff)),
    });
    layout->insertWidget(0, autoWay);
    bindSwitchToMenu(&autoWaySwitch, autoWay);

    MenuWidget* ais = new MenuWidget("",
    {
        new MenuItemValue("",            new OnOffValueWidget(OnOffValueWidget::OnOff)),
        new MenuItemValue("",    new EnumWidget({"", "", "", ""})),
        new MenuItemValue("",    new EnumWidget({"", "", ""})),
    });
    layout->insertWidget(0, ais);
    bindSwitchToMenu(&aisSwitch, ais);

    MenuWidget* installer = new MenuWidget("",
    {
        new MenuItemValue("",    new EnumWidget({"", "", "", ""})),
        new MenuItemValue("",    new EnumWidget({"", ""})),
        new MenuItemValue("", new ValueWidget(0, 1, 0, 9)),
        new MenuItemValue("",       new EnumWidget({"", ""})),
        new MenuItemValue("",new ValueWidget(0, 0, 0, 90)),
        new MenuItemValue("",           new EnumWidget({"", ""})),
        new MenuItemValue("",    new ValueWidget(0, 1, 0, 359.9)),
    });
    layout->insertWidget(0, installer);
    bindSwitchToMenu(&installerSwitch, installer);

    MenuWidget* control = new MenuWidget("",
    {
        new MenuItemValue("",       new OnOffValueWidget(OnOffValueWidget::OnOff)),
        new MenuItemValue("", new OnOffValueWidget(OnOffValueWidget::OnOff)),
        new MenuItemValue("",             new OnOffValueWidget(OnOffValueWidget::OnOff)),
        new MenuItemValue("",            new OnOffValueWidget(OnOffValueWidget::YesNo)),
        new MenuItemValue("",    new ValueWidget(0, 2)),
    });
    layout->insertWidget(0, control);
    bindSwitchToMenu(&controlSwitch, control);

    hide();
}

bool MenuContainer::isVisible() const
{
    return _currMenu->isVisible();
}

void MenuContainer::show()
{
    toDefaultMenu();
    _currMenu->show();
}

void MenuContainer::hide()
{
    _currMenu->hide();
    _currMenu->toFirst();
}

void MenuContainer::toogleVisible()
{
    if (_currMenu->isVisible())
        hide();
    else
        show();
}

void MenuContainer::next()
{
    _currMenu->next();
}

void MenuContainer::prev()
{
    _currMenu->prev();
}

void MenuContainer::toogleEdit()
{
    _currMenu->toogleEdit();
}

void MenuContainer::toDefaultMenu()
{
    setCurrentMenu(_defaultMenu);
}

void MenuContainer::setCurrentMenu(MenuWidget *menu)
{
    bool isShown = _currMenu->isVisible();
    if (isShown)
        hide();

    _currMenu = menu;
    _currMenu->setVisible(isShown);
}

void MenuContainer::bindSwitchToMenu(MenuItemButton *btn, MenuWidget *menu)
{
    connect(btn, &MenuItemButton::startEdit, this, [&,menu]()
    {
        setCurrentMenu(menu);
    });
}
