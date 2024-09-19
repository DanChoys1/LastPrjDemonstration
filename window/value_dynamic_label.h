#ifndef VALUE_DYNAMIC_LABEL_H
#define VALUE_DYNAMIC_LABEL_H

#include "ivalue.h"
#include "styled_classes.h"

class ValueDynamicLabel : public DynamicLabel, public IValue
{ Q_OBJECT
public:
    /// Конструктор
    ValueDynamicLabel(QWidget* p = nullptr) :
        DynamicLabel(p)
    {}
    /// Деструктор
    virtual ~ValueDynamicLabel(){}

    /// Задать значение
    virtual void setValue(qreal val) override
    {
        setPrivateValue(qBound(_min, val, _max));
        emit valueChanged(value());
    }

signals:
    void valueChanged(qreal);

};

#endif // VALUE_DYNAMIC_LABEL_H
