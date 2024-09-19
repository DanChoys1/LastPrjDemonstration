#ifndef IVALUE_H
#define IVALUE_H

#include <cmath>
#include <limits>
#include <QtGlobal>

class IValue
{
public:
    /// Деструктор
    virtual ~IValue(){}

    /// Задать значение
    virtual void setValue(qreal val)
    {
        setPrivateValue(qBound(_min, val, _max));
    }
    /// Значение
    virtual qreal value() const = 0;

    /// Увеличить значение
    virtual void incr()
    {
        setValue(value() + _incrValue);
    };
    /// Уменьшить значение
    virtual void decr()
    {
        setValue(value() - _incrValue);
    }

    /// Точность числа
    virtual void setPercision(int perc)
    {
        if (perc > 0)
        {
            _percision = perc;
            _incrValue = 1.0 / std::pow(10, perc);
        }
        else
        {
            _percision = -1;
            _incrValue = 1;
        }

        setValue(value());
    }

    void setMin(qreal minVal)
    {
        _min = minVal;
        setValue(value());
    }
    void setMax(qreal maxVal)
    {
        _max = maxVal;
        setValue(value());
    }

protected:
    virtual void setPrivateValue(qreal val) = 0;

protected:
    /// Точность значения
    qreal _percision = -1;
    /// Прирост значения
    qreal _incrValue = 1;
    /// Максимальное допустимое для задания значение
    qreal _max = std::numeric_limits<qreal>::max();
    /// Минимальное допустимое для задания значение
    qreal _min = -_max;

};

#endif // IVALUE_H
