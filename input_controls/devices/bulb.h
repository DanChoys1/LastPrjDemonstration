#ifndef BULB_H
#define BULB_H

#include "image_widget.h"

///
/// \brief Лампочка.
/// 2 состояния - горящая, не горящая
///
class Bulb : public ImageWidget
{
    Q_OBJECT

public:
    ///Конструктор
    Bulb(QWidget* parent = nullptr);
    ///Деструктор
    ~Bulb() = default;

    ///Состояние лампочки
    bool isLight() const;

    ///Добавляет изображение
    void addImage(const QString& path);
    ///Добавляет изображение
    void addImage(QPixmap img);

public slots:
    ///Задает свечение
    void setLight(bool isLight);

signals:
    void lightChanged(bool isLight);

private:
    ///Кол-во состояний
    static constexpr int _CountStates = 2;

};

#endif // BULB_H
