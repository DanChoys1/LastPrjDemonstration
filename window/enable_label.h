#ifndef ENABLELABEL_H
#define ENABLELABEL_H

#include <QLabel>

class EnableLabel : public QLabel
{ Q_OBJECT
public:
    EnableLabel(QWidget* parent = nullptr);
    EnableLabel(QString text, bool isEnable = true, QWidget* parent = nullptr);
    void setEnable(bool isEnable);
};

#endif // ENABLELABEL_H
