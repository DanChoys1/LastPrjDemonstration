#ifndef ENUMWIDGET_H
#define ENUMWIDGET_H

#include <QMap>
#include "value_dynamic_label.h"

class EnumWidget : public ValueDynamicLabel
{ Q_OBJECT
public:
    EnumWidget(QWidget* p = nullptr);
    EnumWidget(QMap<int, QString> enums = {}, QWidget* p = nullptr);
    EnumWidget(QMap<int, QString> enums, int val, QWidget* p = nullptr);
    EnumWidget(const QVector<QString> &enums, int val = 0, QWidget* p = nullptr);

    qreal value() const override;
    QString svalue() const;
    /// Увеличить значение
    virtual void incr() override;
    /// Уменьшить значение
    virtual void decr() override;

    void setEnum(QMap<int,QString> e);
    void setEnum(const QVector<QString> &e);

protected:
    void setPrivateValue(qreal val) override;

private:
    QMap<int,QString> _enum;
    QMap<int,QString>::ConstIterator _currItem;

};

#endif // ENUMWIDGET_H
