#include "menu_widget.h"

#include <QVBoxLayout>

MenuWidget::MenuWidget(QString header, QVector<MenuItem*> items, bool isVisible, QWidget *p) :
    QWidget(p)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(&_header);
    _header.setText(header);
    _header.setAlignment(Qt::AlignCenter);

    if (!items.isEmpty())
        setItems(items);

    setVisible(isVisible);
}

void MenuWidget::next()
{
    if (_itemIt.hasNext() && !_isEdit)
    {
        if (_itemIt.hasPrevious())
            _itemIt.peekPrevious()->unselect();

        while(_itemIt.hasNext() && !_itemIt.next()->isEnable());
        if (_itemIt.peekPrevious()->isEnable())
        {
            _itemIt.peekPrevious()->select();
        }
    }
    else if (_itemIt.hasPrevious() && _itemIt.peekPrevious()->isEnable())
    {
        _itemIt.peekPrevious()->decr();
    }
}

void MenuWidget::prev()
{
    if (_itemIt.hasPrevious() && !_isEdit)
    {
        if (_itemIt.hasPrevious())
            _itemIt.previous()->unselect();

        while(_itemIt.hasPrevious() && !_itemIt.previous()->isEnable());
        if (_itemIt.peekNext()->isEnable())
        {
            _itemIt.next()->select();
        }
    }
    else if (_itemIt.hasPrevious() && _itemIt.peekPrevious()->isEnable())
    {
        _itemIt.peekPrevious()->incr();
    }
}

void MenuWidget::toFirst()
{
    if (_itemIt.hasPrevious())
    {
        setEdit(false);
        _itemIt.previous()->unselect();
    }

    _itemIt.toFront();
    next();
}

void MenuWidget::setEdit(bool isEdit)
{
    if (!_itemIt.hasPrevious()) return;

    _isEdit = isEdit;
    if (_isEdit)
        _itemIt.peekPrevious()->edit();
    else
        _itemIt.peekPrevious()->select();
}

bool MenuWidget::isEdit() const
{
    return _isEdit;
}

void MenuWidget::toogleEdit()
{
    setEdit(!isEdit());
}

void MenuWidget::startEdit()
{
    setEdit(true);
}

void MenuWidget::stopEdit()
{
    setEdit(false);
}

void MenuWidget::setItems(QVector<MenuItem *> items)
{
    _items = items;
    _itemIt = _items;

    QLayout* l = layout();
    foreach(auto item, _items)
    {
        l->addWidget(item);
        // Если выделенный элемент становится unenable,
        // меню переключается на следующий или предыдущий элемент
        connect(item, &MenuItem::unenable, this, [&,item]()
        {
            if (_itemIt.previous() != item) return;
            _isEdit = false;
            if (_itemIt.hasNext())
                next();
            else
                prev();
        });
    }

    toFirst();
}
