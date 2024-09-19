#include "image_widget.h"

#include <QResizeEvent>
#include <QtDebug>
#include <QtMath>

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent)
{}

ImageWidget::ImageWidget(const QString &path, QWidget *parent) :
    ImageWidget(parent)
{
    addImage(path);
}

ImageWidget::~ImageWidget()
{
    _imgsParametrs.clear();
}

void ImageWidget::addImage(const QString &path)
{
    addImage(QPixmap(path));
}

void ImageWidget::addImage(QPixmap img)
{
    addImage(ImageParametrs(std::move(img), true));
}

void ImageWidget::addImage(ImageParametrs img)
{
    assert(!img.img.isNull() && "QPixmap пустой"); //Если нет изображения - ошибка

    _imgsParametrs.append(std::move(img));
    update(); //Перерисовываем
}

QSize ImageWidget::imgSize(int index)
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    return _imgsParametrs.at(index).img.size();
}

int ImageWidget::count() const
{
    return _imgsParametrs.count();
}

void ImageWidget::remove(int index)
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    _imgsParametrs.removeAt(index);

    update(); //Перерисовываем
}

const ImageParametrs& ImageWidget::imageParametrs(int index) const
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    return _imgsParametrs[index];
}

void ImageWidget::setImageRotation(qreal rotation, qsizetype index)
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    if (!qFuzzyCompare(_imgsParametrs[index].rotation, rotation))
    {
        _imgsParametrs[index].rotation = rotation;
        update(); //Перерисовываем
    }
}

qreal ImageWidget::imageRotation(qsizetype index) const
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    return _imgsParametrs[index].rotation;
}

QPoint ImageWidget::imagePos(int index) const
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    return _imgsParametrs[index].pos();
}

qreal ImageWidget::imageHPos(int index) const
{
    return _imgsParametrs[index].x;
}

qreal ImageWidget::imageVPos(int index) const
{
    return _imgsParametrs[index].y;
}

void ImageWidget::setImagePos(QPoint newPos, int index)
{
    setImagePos(newPos.x(), newPos.y(), index);
}

void ImageWidget::setImagePos(int x, int y, int index)
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    _imgsParametrs[index].x = x;
    _imgsParametrs[index].y = y;
    update();
}

void ImageWidget::setImageHPos(qreal x, int index)
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    _imgsParametrs[index].x = x;
    update();
}

void ImageWidget::setImageVPos(qreal y, int index)
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    _imgsParametrs[index].y = y;
    update();
}

void ImageWidget::hide()
{
    emit hidden();
    QWidget::hide();
}

void ImageWidget::show()
{
    emit shown();
    QWidget::show();
}

void ImageWidget::hideImage(int index)
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    _imgsParametrs[index].isShow = false;
     update(); //Перерисовываем
}

void ImageWidget::hideImages()
{
    for (int i = 0; i < _imgsParametrs.count(); ++i)
    {
      _imgsParametrs[i].isShow = false;
    }
}

void ImageWidget::showImage(int index)
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    _imgsParametrs[index].isShow = true;
    update(); //Перерисовываем
}

void ImageWidget::showOnlyImage(int index)
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    hideImages();
    showImage(index);
}

bool ImageWidget::isImageShow(int index) const
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    return _imgsParametrs[index].isShow;
}

void ImageWidget::setImageCenter(QPoint center, int index)
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");
    _imgsParametrs[index].center = center;
}

void ImageWidget::setCountStates(qsizetype count, qsizetype index)
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");
    assert(count > 0 && "Кол-во состояний меньше или равно 0");

    _imgsParametrs[index].countStates = count;
}

void ImageWidget::setState(qsizetype state, qsizetype index)
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");
    assert(state >= 0 && "Состояние не может быть меньше 0");

    if (_imgsParametrs[index].state != state)
    {
      _imgsParametrs[index].state = state;
      update();
    }
}

qsizetype ImageWidget::state(qsizetype index) const
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    return _imgsParametrs[index].state;
}

uint ImageWidget::countStates(qsizetype index) const
{
    assert(_imgsParametrs.count() > index && "Выход за границы массива");

    return _imgsParametrs[index].countStates;
}

void ImageWidget::setCyclicity(bool isCyclicity)
{
    _isCyclicity = isCyclicity;
}

void ImageWidget::setPos(QPoint newPos)
{
    setPos(newPos.x(), newPos.y());
}

void ImageWidget::setPos(int x, int y)
{
    QRect parentRect = this->parentWidget()->rect();

    // Выходит за левую границу -> x = левая граница
    x = x > parentRect.left() ? x : parentRect.left();
    // Выходит за правую границу -> x = правая граница
    x = (x + this->width()) < parentRect.right() ? x : parentRect.right() - this->width();

    // Выходит за вырхнюю границу -> y = вырхняя граница
    y = y > parentRect.top() ? y : parentRect.top();
    // Выходит за нижнюю границу -> y = нижняя граница
    y = (y + this->height()) < parentRect.bottom() ? y : parentRect.bottom() - this->height();

    this->move(x, y);
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    for (ImageParametrs& imgParametrs : _imgsParametrs)
    {
        if (!imgParametrs.isShow) continue;

        painter.setTransform(imgParametrs.transform());
        painter.drawPixmap(0, 0, imgParametrs.img);

        if (_isCyclicity)
        {
            if (imgParametrs.x > 0)
            {
                painter.drawPixmap(-imgParametrs.img.width(), 0, imgParametrs.img);
            }
            else if (-imgParametrs.x > (imgParametrs.img.width() - this->width()))
            {
                painter.drawPixmap(imgParametrs.img.width(), 0, imgParametrs.img);
            }

            if (imgParametrs.y > 0)
            {
                painter.drawPixmap(0, -imgParametrs.img.height(), imgParametrs.img);
            }
            else if (-imgParametrs.y > (imgParametrs.img.height() - this->height()))
            {
                painter.drawPixmap(0, imgParametrs.img.height(), imgParametrs.img);
            }
        }
    }

    QWidget::paintEvent(event);
}

///
/// Методы класса Параметров Изображения
///

ImageParametrs::ImageParametrs(QPixmap img, bool isShow,
                               qreal rotation, qreal x,
                               qreal y, qreal countState,
                               qreal state)
{
    this->img = std::move(img);
    this->isShow = isShow;    
    this->countStates = countState;
    this->state = state;

    this->rotation = rotation;
    this->x = x;
    this->y = y;
}

ImageParametrs::~ImageParametrs()
{
    img = QPixmap();
    center = QPoint();
    isShow = false;
    countStates = 0;
    state = 0;
    rotation = 0;
    x = 0;
    y = 0;
}

QTransform ImageParametrs::transform() const
{
    assert(countStates > 0 && "Кол-во состояний должно быть больше 0");

//    qreal height = img.height();
    qreal width = img.width() / (qreal)countStates; //Ширина одного состояния

    qreal posX = x - width * state;
    qreal posY = y;

    qreal rotPosX = width / 2 + center.x();
    qreal rotPosY = img.height() / 2 + center.y();

    return QTransform()
             .translate(posX, posY) //Устанавливаем в изображение на позицию указанного состояния
             .translate(rotPosX, rotPosY) //Устанавливаем координаты в центр изображения
             .rotate(rotation) //Поворачиваем изображение
             .translate(-rotPosX, -rotPosY); //Возвращаем координаты;
}

QPoint ImageParametrs::pos() const
{
    return QPoint(x, y);
}
