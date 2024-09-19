#ifndef NOISE_EMULATOR_H
#define NOISE_EMULATOR_H

#include <QGraphicsItem>
#include <QRadialGradient>

#include "flag.h"
#include "target_data.h"
#include "simplex_noise_generator.h"

/// Отрисовка шумов рандомных вокруг, целей и их следов
class NoiseEmulator : public QGraphicsObject
{ Q_OBJECT
public:
    NoiseEmulator(qreal size, QGraphicsItem *parent = nullptr);

    /// Отображать шум
    void setSearchPower(bool isOn);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    /// Скалирование масштаба
    void setCoordinatesScaleRatio(qreal scale);
    /// Обновить угол отображения шума
    void update(qreal bearing, const QVector<RlsData::Target>& targets = {});

    /// Задать позицию носителя
    void setSelfPos(RectPoint pos);
    /// Смещение отрисовки изображения
    void setCenterShift(RectPoint shift);

    /// Отображать следы целей
    void setIsShowTraces(bool isShow);

    /// Поворот шумов
    void setRotation(qreal a);

private:
    /// Отрисовка всего шума
    void paintBearing(qreal bearing, qreal slope, RectPoint currImgPos);
    /// Обновление шума целей
    void updateTargets(const QVector<RlsData::Target>& targets, RectPoint currImgPos);

    /// Генерация шума
    void generateNextNoiseMapThread(RectPoint center);
    /// Генерация шума
    void generateNextNoiseMap(RectPoint center);

    /// Немного двигает шум туда сюда, чтобы был эфект луча когда корабль не движется
    void generateStaticNoiseMovement();

signals:
    void noiseGenereted(QImage img);

private slots:
    void setGeneratedNoise(QImage img);

private:
    /// Областьт отрисовки
    qreal _size = 0;

    /// Флаг на первое изменение масштаба. Первое изменение масшатаба игнорируем
    Flag _firstScaleFlag { Flag::IsNotChecked };
    /// Масштаб шума
    qreal _scale = 1;

    /// Шум который отрисован
    QPixmap _showImg;
    /// Сгенерированный рандомный шума
    QImage _noiseImg;
    /// Шум целей
    QImage _targetImg;

    /// Градиент для затенения краев
    QRadialGradient _transparentGradient;

    /// Генератор шумов
    SimplexNoiseGenerator _noiseGenerator;

    /// Поворот шумов
    qreal _rotation = 0;

    /// Позиция центра изображения шума
    RectPoint _currCenter;
    /// Позиция центра следующего сгенерированного изображения шума
    RectPoint _nextCenter;
    /// Позиция носителя
    RectPoint _selfPos;
    /// Смещение центра отрисовки
    RectPoint _centerShift;
    /// На сколько будет двигаться статический шум
    RectPoint _staticNoiseMove {1,1};

    /// Пеленг отрисовки изображения
    qreal _bearing = 0;

    /// Флаг отрисовки шума
    bool _isOn = false;

    /// Флаг отрисовки следов
    bool _isShowTraces = false;

    /// Поток в котором генерируется рандомный шум
    QThread* _noiseGenThread = nullptr;

};

#endif // NOISE_EMULATOR_H
