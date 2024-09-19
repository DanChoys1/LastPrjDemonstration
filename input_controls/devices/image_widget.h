#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMap>
#include <tuple>
#include <memory>

struct ImageParametrs;

///
/// \brief The ImageWidget class - Виджет с изображениями
///
class ImageWidget : public QWidget
{
    Q_OBJECT

public:    
    ///Конструктор
    ImageWidget(QWidget* parent = nullptr);
    ///Конструктор
    ImageWidget(const QString &path, QWidget* parent = nullptr);
    ///Деструктор
    ~ImageWidget();

    ///Добавляет изображение
    void addImage(const QString& path);
    ///Добавляет изображение
    void addImage(QPixmap img);
    ///Добавляет изображение
    void addImage(ImageParametrs img);

    ///Размер изображения
    QSize imgSize(int index = 0);
    ///Кол-во изображений
    int count() const;
    ///Удаляет Изображение
    void remove(int index = 0);

    ///Получить параметры изображения
    const ImageParametrs& imageParametrs(int index) const;

    ///Задать поворот
    void setImageRotation(qreal rotation, qsizetype index = 0);
    ///Возвращает поворот
    qreal imageRotation(qsizetype index = 0) const;

    ///Позиция изображения
    QPoint imagePos(int index = 0) const;
    ///Позиция изображения по горизонтали
    qreal imageHPos(int index = 0) const;
    ///Позиция изображения по вертикали
    qreal imageVPos(int index = 0) const;
    ///Перемещение изображения
    void setImagePos(QPoint newPos, int index = 0);
    ///Перемещение изображения
    void setImagePos(int x, int y, int index = 0);
    ///Перемещение изображения по горизонтали
    void setImageHPos(qreal x, int index = 0);
    ///Перемещение изображения по вертикали
    void setImageVPos(qreal y, int index = 0);

    ///Скрыть виджет
    void hide();
    ///Отобразить виджет
    void show();
    ///Скрыть изображение
    void hideImage(int index = 0);
    ///Скрывает все изображения
    void hideImages();
    ///Отобразить изображение
    void showImage(int index = 0);
    ///Отображать только изображение по индексу
    void showOnlyImage(int index);
    ///Видно ли изображение по индексу
    bool isImageShow(int index = 0) const;

    void setImageCenter(QPoint center, int index = 0);

    ///Задать кол-во состояний
    void setCountStates(qsizetype count, qsizetype index = 0);
    ///Задать состояние
    void setState(qsizetype state, qsizetype index = 0);
    ///Состояние
    qsizetype state(qsizetype index = 0) const;
    ///Кол-во состояний
    uint countStates(qsizetype index = 0) const;

    ///Включить цикличность перемещения изображения
    void setCyclicity(bool isCyclicity);

    ///Перемещение виджета в области родителя
    void setPos(QPoint newPos);
    ///Перемещение виджета в области родителя
    void setPos(int x, int y);

signals:
//    void mousePressed(QMouseEvent *event);
//    void mouseReleased(QMouseEvent *event);
//    void mouseMoved(QMouseEvent *event);

    ///Событие скрытия
    void hidden();
    ///Событие отображения
    void shown();

    void rotationChanged(qreal val);
    void rotationChangedTo(qreal diffVal);
    void posChanged(int x, int y);
    void posChangedTo(int diffX, int diffY);

protected:
    ///Отрисовка
    void paintEvent(QPaintEvent *event) override;

//    void mousePressEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);

private:
    ///Изображения
    QVector<ImageParametrs> _imgsParametrs;

    ///Изображение повторяется при выходе из области видимости
    /// !!! Не полностью роботает, нодо доработать(вроде) :( !!!
    bool _isCyclicity = false;


};

///
/// \brief ImageParametrs - Параметры изображения
///
struct ImageParametrs
{
    ///Изображение
    QPixmap img = QPixmap();
    ///Отображается ли
    bool isShow = false;

    qreal x { 0 };
    qreal y { 0 };
    qreal rotation { 0 };
    QPoint center;
    ///Состояние
    qreal state = 0;
    ///Кол-во состояний
    int countStates = 1;

    ///Конструктор
    ImageParametrs() = default;
    ///Конструктор
    ImageParametrs(QPixmap img, bool isShow = false,
                   qreal rotation = 0, qreal x = 0,
                   qreal y = 0, qreal countState = 1,
                   qreal state = 0);
    ///Деструктор
    ~ImageParametrs();

    ///Трансформация
    QTransform transform() const;

    QPoint pos() const;

};

#endif // IMAGEWIDGET_H
