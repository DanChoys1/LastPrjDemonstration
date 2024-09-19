#ifndef VIRTUAL_KEYBOARD_H
#define VIRTUAL_KEYBOARD_H

#include "input_control.h"

namespace Ui {
class VirtualKeyboard;
}

/// Вирутальная клавиатура ввода
class VirtualKeyboard : public InputControl
{ Q_OBJECT
public:
    explicit VirtualKeyboard(QWidget *parent = nullptr);
    ~VirtualKeyboard();

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    /// Настройка виджетов ввода
    void initDevices();
    /// Конект событий взаимодействия с виджетами мужду виджетами виртуальной клавиатуры
    void connectDevises();
    /// Конект событий взаимодействия с виджетами
    void connectSignals();

private slots:
    /// Включение питания клавиатуры
    void powerChanged(bool isOn);

    /// Переключение развертки
    void switchReview();
    /// Переключение выноса Визира
    void switchVnVd();

    /// Подключение/Отключение событий управления меню
    void reconnectMenu();

private:
    /// Окно отображения клавиатуры
    Ui::VirtualKeyboard *ui;

    /// Флаг открития меню
    bool _isMenuOpen = true;

    /// Пустое изображение для скрытия курсора
    QPixmap cursorHidePixmap = QPixmap(1,1);
    /// Позиция зажатия курсором на манипулятор
    QPoint _cursorPressedPos ;

};

#endif // VIRTUAL_KEYBOARD_H
