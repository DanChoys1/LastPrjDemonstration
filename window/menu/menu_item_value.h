#ifndef MENUITEMVALUE_H
#define MENUITEMVALUE_H

#include "menu_item.h"
#include "enable_label.h"
#include "value_dynamic_label.h"

class MenuItemValue : public MenuItem
{ Q_OBJECT
public:
    MenuItemValue(QString header, ValueDynamicLabel* value, bool isEnable = true, QWidget* p = nullptr);

    void setEnable(bool isEnable) override;

    void incr() override;
    void decr() override;
    void setValue(qreal val);
    qreal value() const;

    void repolish() override;

signals:
    void valueChanged(qreal val);

private:
    EnableLabel _header;
    ValueDynamicLabel* _value = nullptr;

};

#endif // MENUITEMVALUE_H
