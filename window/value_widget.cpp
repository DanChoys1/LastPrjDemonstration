#include "value_widget.h"

ValueWidget::ValueWidget(QWidget *p) :
    ValueDynamicLabel(p)
{}

ValueWidget::ValueWidget(qreal val, int percision, QWidget *p) :
    ValueWidget(p)
{
    setPercision(percision);
    setValue(val);
}

ValueWidget::ValueWidget(qreal val, int percision, qreal min, qreal max,QWidget *p) :
    ValueWidget(val, percision, p)
{
    setMin(min);
    setMax(max);
}

void ValueWidget::setPrivateValue(qreal val)
{
    _val = val;
    if (_percision < 0)
    {
        setText(QString::number(_val));
    }
    else
    {
        // Т.к. QString::number округляет числа в большую сторону при >=0.5, написал свое округление в меньшую сторону
        int precNum = pow(10,_percision);               // Разрад число до которого округляем
        qreal roundVal = trunc(_val*precNum) / precNum; // Округленное до точности _percision число
        setText(QString::number(roundVal, 'f', _percision));
    }
}

qreal ValueWidget::value() const
{
    return _val;
}
