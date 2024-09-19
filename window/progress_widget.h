#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QFrame>

class ProgressWidget : public QFrame
{ Q_OBJECT
public:
    ProgressWidget(QWidget* p = nullptr);

    void setValue(qreal val);
    qreal value() const;

private:
    qreal validValue(qreal val) const;

private:
    qreal _val = 0;

};

#endif // PROGRESSWIDGET_H
