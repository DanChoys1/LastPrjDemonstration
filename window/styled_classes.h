#ifndef CONTUREDFIELD_H
#define CONTUREDFIELD_H

#include <QFrame>
#include <QLabel>

class ConturedFrame : public QFrame
{ Q_OBJECT
public:
    ConturedFrame(QWidget* parent = nullptr) : QFrame(parent) {}
};

class ConturedNotLeftFrame : public ConturedFrame
{ Q_OBJECT
public:
    ConturedNotLeftFrame(QWidget* parent = nullptr) : ConturedFrame(parent) {}
};

class ConturedLabel : public QLabel
{ Q_OBJECT
public:
    ConturedLabel(QWidget* parent = nullptr) : QLabel(parent) {}
    ConturedLabel(QString text, QWidget* parent = nullptr) : QLabel(text, parent) {}
};

class ConturedBottomLabel : public QLabel
{ Q_OBJECT
public:
    ConturedBottomLabel(QWidget* parent = nullptr): QLabel(parent) {}
    ConturedBottomLabel(QString text, QWidget* parent = nullptr): QLabel(text, parent) {}
};

class DynamicLabel : public QLabel
{ Q_OBJECT
public:
    DynamicLabel(QWidget* parent = nullptr) : QLabel(parent) {}
};

#endif // CONTUREDFIELD_H
