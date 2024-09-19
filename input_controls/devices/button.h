#ifndef BUTTON_H
#define BUTTON_H

#include "image_widget.h"

///
/// \brief Кнопка.
/// 4 состояния - отжата, нажата, отжата горящая, нажата горящая.
///
class Button : public ImageWidget
{
    Q_OBJECT

public:
    ///Конструктор
    Button(QWidget* parent = nullptr);
    ///Деструктор
    ~Button();

    ///Включить возможность подстветки
    void setHighlighting(bool  isHighlighting);

    ///Свечение
    bool isLight() const;

    ///Добавляет изображение
    void addImage(const QString& path);
    ///Добавляет изображение
    void addImage(QPixmap img);

    bool isPressed() const;

public slots:
    ///Задать подсветку
    void setLight(bool isLight);

signals:
    ///Нажата
    void pressed();
    ///Отжата
    void released();
    ///Подсветка
    void light(bool);

protected:
    ///Нажатие
    void mousePressEvent(QMouseEvent*) override;
    ///Отпустили
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    ///Подсвечена
    bool _isLight = false;
    ///Возможность подсветки
    bool _isHighlighting = false;

    ///Нажата ли кнопка
    bool _isPressed = false;

    ///Кол-во состояний при выкл возможности подсветки
    static constexpr int _CountStates = 2;
    ///Кол-во состояний при вкл возможности подсветки
    static constexpr int _CountHighlightingStates = 4;

};   

#endif // BUTTON_H
