#ifndef VALUEWIDGET_H
#define VALUEWIDGET_H

#include "value_dynamic_label.h"

class ValueWidget : public ValueDynamicLabel
{ Q_OBJECT
public:
    ValueWidget(QWidget* p = nullptr);
    ValueWidget(qreal val, int percision = -1, QWidget* p = nullptr);
    ValueWidget(qreal val, int percision, qreal min, qreal max, QWidget* p = nullptr);

    qreal value() const override;

protected:
    void setPrivateValue(qreal val) override;

private:
    qreal _val = 0;

};

#endif // VALUEWIDGET_H
