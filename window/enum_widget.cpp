#include "enum_widget.h"

#include <QtDebug>

EnumWidget::EnumWidget(QWidget *p) :
    ValueDynamicLabel(p)
{}

EnumWidget::EnumWidget(QMap<int, QString> enums, QWidget* p) :
    EnumWidget(p)
{
    setEnum(enums);
}

EnumWidget::EnumWidget(QMap<int, QString> enums, int val, QWidget *p) :
    EnumWidget(enums, p)
{
    setValue(val);
}

EnumWidget::EnumWidget(const QVector<QString>& enums, int val, QWidget *p)
{
    setEnum(enums);
    setValue(val);
}

void EnumWidget::setPrivateValue(qreal val)
{
    auto item = _enum.constFind((int)val);
    if (item != _enum.constEnd())
    {
        _currItem = item;
        setText(svalue());
    }
    else
    {
        qDebug() << QStringLiteral("EnumWidget::setPrivateValue не найден элемент val");
    }
}

qreal EnumWidget::value() const
{
    return _currItem.key();
}

QString EnumWidget::svalue() const
{
    return _currItem.value();
}

void EnumWidget::incr()
{
    auto item = _enum.constFind(value());
    if (++item != _enum.end())
        setValue(item.key());
}

void EnumWidget::decr()
{
    auto item = _enum.constFind(value());
    if (item != _enum.begin())
        setValue((--item).key());
}

void EnumWidget::setEnum(QMap<int, QString> e)
{
    if (e.isEmpty())
        return;

    _enum = e;
    setValue(_enum.firstKey());
}

void EnumWidget::setEnum(const QVector<QString>& e)
{
    _enum.clear();
    for(int i = 0; i < e.count(); ++i)
    {
        _enum[i] = e[i];
    }

    setValue(_enum.firstKey());
}
