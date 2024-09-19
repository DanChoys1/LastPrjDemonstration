#ifndef TUMBLER_H
#define TUMBLER_H

#include "image_widget.h"

///
/// \brief Тумблер
///
class Tumbler : public ImageWidget
{
    Q_OBJECT

public:
    ///Конструктор
    Tumbler(QWidget* parent = nullptr);
    ///Деструктор
    ~Tumbler() = default;

    ///Включен ли
    bool isOn() const;

    ///Добавляет изображение
    void addImage(const QString& path);
    ///Добавляет изображение
    void addImage(QPixmap img);

signals:
    ///Событие включения
    void on();
    ///Событие выключения
    void off();

    void changeState(bool isOn);

protected:
    ///Событие нажатия мыши
    void mouseReleaseEvent(QMouseEvent *event);

private:
    ///Кол-во состояний
    static constexpr int _CountStates = 2;

};

#endif // TUMBLER_H
