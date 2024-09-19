#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include "menu_item.h"
#include "styled_classes.h"

#include <QVector>

class MenuWidget : public QWidget
{ Q_OBJECT
public:
    MenuWidget(QString header = "", QVector<MenuItem*> items = {}, bool isVisible = false, QWidget* p = nullptr);

    void next();
    void prev();
    void toFirst();

    void setEdit(bool isEdit);
    bool isEdit() const;
    void toogleEdit();
    void startEdit();
    void stopEdit();

    void setItems(QVector<MenuItem*> items);

private:
    QLabel _header;
    QVector<MenuItem*> _items;
    QVectorIterator<MenuItem*> _itemIt { _items };

    bool _isEdit = false;

};

#endif // MENUWIDGET_H
