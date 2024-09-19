#ifndef ONOFFVALUEWIDGET_H
#define ONOFFVALUEWIDGET_H

#include "enum_widget.h"

class OnOffValueWidget : public EnumWidget
{ Q_OBJECT
public:
    enum Type
    {
        OnOff,
        YesNo
    };

    OnOffValueWidget(QWidget* p = nullptr);
    OnOffValueWidget(Type t, QWidget* p = nullptr);
    OnOffValueWidget(bool isOn, Type t, QWidget* p = nullptr);

    void setType(Type t);
};

#endif // ONOFFVALUEWIDGET_H
