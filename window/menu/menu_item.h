#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <QFrame>

class MenuItem : public QFrame
{ Q_OBJECT
public:
    enum State
    {
        Unselect,
        Select,
        Edit
    };

    MenuItem(QWidget* p = nullptr);
    virtual ~MenuItem();

    void setState(State state);
    State state() const;

    void unselect();
    void select();
    virtual void repolish();
    virtual void edit();

    virtual void setEnable(bool isEnable);
    bool isEnable() const;

    virtual void incr() {};
    virtual void decr() {};

signals:
    void startEdit();
    void stopEdit();
    void selected();
    void unselected();

    void unenable();
    void enable();

protected:
    virtual void repolish(QWidget* self);

private:
    State _state = Unselect;
    bool _isEnable = true;

};

#endif // MENU_ITEM_H
