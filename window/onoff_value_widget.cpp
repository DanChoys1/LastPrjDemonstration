#include "onoff_value_widget.h"

OnOffValueWidget::OnOffValueWidget(QWidget *p) :
    EnumWidget(p)
{
    setType(OnOff);
}

OnOffValueWidget::OnOffValueWidget(Type t, QWidget *p) :
    EnumWidget({}, p)
{
    setType(t);
}

OnOffValueWidget::OnOffValueWidget(bool isOn, Type t, QWidget *p) :
    OnOffValueWidget(t, p)
{
    setValue(isOn);
}

void OnOffValueWidget::setType(Type t)
{
    switch (t)
    {
    case OnOff:
        setEnum({{0, QStringLiteral("ОТКЛ")}, {1, QStringLiteral("ВКЛ")} });
        break;
    case YesNo:
        setEnum({{0, QStringLiteral("НЕТ")}, {1, QStringLiteral("ДА")} });
        break;
    }
}
