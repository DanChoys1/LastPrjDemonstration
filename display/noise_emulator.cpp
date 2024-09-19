#include "noise_emulator.h"

#include "geometry_math.h"
#include "uisettings.h"

#include <QPainter>
#include <QRandomGenerator>
#include <QThread>

namespace
{
    constexpr int    InitScale         = 1000;
    constexpr int    InitOctaves       = 10;
    constexpr double InitPersistence   = 0.7;
    constexpr double InitLacunarity    = 2;
}

NoiseEmulator::NoiseEmulator(qreal size, QGraphicsItem *parent) :
    QGraphicsObject(parent), _size(size*2), _showImg(_size, _size),
    _noiseImg(_size, _size, QImage::Format::Format_ARGB32),
    _targetImg(_size, _size, QImage::Format::Format_ARGB32),
    _noiseGenerator(InitOctaves, InitPersistence, InitLacunarity, qrand()),
    _transparentGradient({_size/2, _size/2}, _size/2)
{
    _transparentGradient.setColorAt(0, QColor(0,0,0,0));
    _transparentGradient.setColorAt(1, QColor(0,0,0,255));

    connect(this, &NoiseEmulator::noiseGenereted, this, &NoiseEmulator::setGeneratedNoise);
}

void NoiseEmulator::setSearchPower(bool isOn)
{
    _isOn = isOn;
}

QRectF NoiseEmulator::boundingRect() const
{
    return { GeometryMath::centeredRectF(_size/2) };
}

void NoiseEmulator::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QBrush brush {_showImg};
    brush.setTransform(QTransform().translate(-_size/2, -_size/2));
    painter->setBrush(brush);
    painter->drawEllipse(-_size / 4, -_size / 4, _size/2, _size/2);
}

void NoiseEmulator::setCoordinatesScaleRatio(qreal scale)
{
    // Первое изменение масшатаба игнорируем
    // т.к. считаем сгенерированное изображение шума в масштабе 1 к текущему масштабу обстановки
    if (!_firstScaleFlag)
        _scale *= scale;

    _selfPos *= scale;
    _nextCenter *= scale;
    _currCenter *= scale;
}

void NoiseEmulator::update(qreal bearing, const QVector<RlsData::Target>& targets)
{
    if (bearing < _bearing)
    {
        _targetImg.fill(Qt::transparent);
        paintBearing(_bearing, 360 - _bearing, _currCenter - _selfPos + _centerShift);
        _bearing = 0;

        generateStaticNoiseMovement();
    }

    // Шум целей
    updateTargets(targets, _currCenter - _selfPos + _centerShift);
    // Шум
    paintBearing(_bearing, bearing - _bearing, _currCenter - _selfPos + _centerShift);

    _bearing = bearing;
}

void NoiseEmulator::setSelfPos(RectPoint pos)
{
    _selfPos = pos;

    qreal distToSelf = CoordinateMath::distance(_nextCenter - _selfPos + _centerShift) / _scale;
    qreal maxDist = _size/2 * (2/4.0);
    if (distToSelf > maxDist)
    {
        generateNextNoiseMapThread(_selfPos);
    }
}

void NoiseEmulator::setCenterShift(RectPoint shift)
{
    _centerShift = shift;
}

void NoiseEmulator::setIsShowTraces(bool isShow)
{
    _isShowTraces = isShow;
}

void NoiseEmulator::setRotation(qreal a)
{
    _rotation = a;
}

void NoiseEmulator::paintBearing(qreal bearing, qreal slope, RectPoint currImgPos)
{
    QPainter areaNoisePainter(&_showImg);
    areaNoisePainter.setPen(Qt::NoPen);
    areaNoisePainter.setRenderHints(QPainter::SmoothPixmapTransform, true);

    qreal xShift = _centerShift.x;
    qreal yShift = _centerShift.y;

    bearing -= 1 - _rotation;
    slope += 2;

    areaNoisePainter.setBrush({ Qt::black });
    areaNoisePainter.drawPie(xShift, yShift, _size, _size, 90*16-bearing*16, -slope*16);

    // Если прибор включен и _noiseImg не перезаписывается. Блокируем перезапись _noiseImg
    if (_isOn)
    {
        // Рандомный шум
        RectPoint noisePos = _staticNoiseMove + currImgPos;
        QBrush noiseBrush { _noiseImg };

        noiseBrush.setTransform(QTransform()
                                .translate(_size/2 + noisePos.x, _size/2 + noisePos.y)
                                .rotate(_rotation)
                                .translate(-_size/2 - noisePos.x, -_size/2 - noisePos.y)
                                .scale(_scale, _scale)
                                .translate(_size/2 * (1/_scale - 1), _size/2 * (1/_scale - 1))
                                .translate(noisePos.x/_scale, noisePos.y/_scale));
        areaNoisePainter.setBrush(noiseBrush);
        areaNoisePainter.drawPie(xShift, yShift, _size, _size, 90*16-bearing*16, -slope*16);

        // Цели
        QBrush targetsBrush { _targetImg };
        targetsBrush.setTransform(QTransform()
                                  .scale(_scale, _scale)
                                  .translate(_size/2 * (1/_scale - 1), _size/2 * (1/_scale - 1))
                                  .translate(currImgPos.x/_scale, currImgPos.y/_scale));
        areaNoisePainter.setBrush(targetsBrush);
        areaNoisePainter.drawPie(xShift, yShift, _size, _size, 90*16-bearing*16, -slope*16);

        // Затенение шума
        QBrush transparentMatrixBrush(_transparentGradient);
        transparentMatrixBrush.setTransform(QTransform()
                                            .scale(_scale, _scale)
                                            .translate(_size/2 * (1/_scale - 1), _size/2 * (1/_scale - 1))
                                            .translate(xShift/_scale, yShift/_scale));
        areaNoisePainter.setBrush(transparentMatrixBrush);
        areaNoisePainter.drawPie(xShift, yShift, _size, _size, 90*16-bearing*16, -slope*16);
    }
}

void NoiseEmulator::updateTargets(const QVector<RlsData::Target> &targets, RectPoint currImgPos)
{
    if (targets.isEmpty())
        return;

    QPainter tarNoisePainter(&_targetImg);
    tarNoisePainter.setPen(Qt::NoPen);
    tarNoisePainter.setBrush(_isShowTraces ? UISettings::TargetsNoiseTracesColor : UISettings::TargetsNoiseColor);
    tarNoisePainter.translate(QPointF{_size / 2.0, _size / 2.0} + (_centerShift - currImgPos) / _scale);
    tarNoisePainter.rotate(_rotation);
    // Шум цели
    foreach(const auto& target, targets)
    {
        if (target.isLost)
            continue;

        QPointF pos = target.relativeRecPos / _scale;
        tarNoisePainter.translate(pos);

        // Отрисовка шума с учетом ракурса цели
        qreal viewRotate = GeometryMath::to180(target.bear);
        tarNoisePainter.rotate(viewRotate);
        tarNoisePainter.drawEllipse(QPointF{0, 0}, UISettings::TargetsNoiseSize + UISettings::TargetsNoiseViewScale * target.viewCoeff,
                                                   UISettings::TargetsNoiseSize);
        tarNoisePainter.rotate(-viewRotate);

        // Следы цели
        if (_isShowTraces)
        {
            foreach(const auto& dot, target.dots)
            {
                tarNoisePainter.drawEllipse(dot / _scale, UISettings::TargetsNoiseSize, UISettings::TargetsNoiseSize);
            }
        }

        tarNoisePainter.translate(-pos);
    }
}

void NoiseEmulator::generateNextNoiseMapThread(RectPoint center)
{
    _nextCenter = center;

//    if (_noiseGenThread != nullptr && _noiseGenThread->isRunning())
//        _noiseGenThread->terminate();

    QThread* noiseGenThread = QThread::create(std::bind(&NoiseEmulator::generateNextNoiseMap, this, center));
    noiseGenThread->start();
}

void NoiseEmulator::generateNextNoiseMap(RectPoint center)
{
    qreal shiftX = center.x / _scale;
    qreal shiftY = center.y / _scale;
    QImage resImg(_size, _size, QImage::Format_ARGB32);
    auto noiseLambda = [&](qreal xS, qreal yS, qreal xE, qreal yE)
    {
        for (int i = xS; i < xE; ++i)
        {
            for (int j = yS; j < yE; ++j)
            {
                qreal x = (i + shiftX) / InitScale;
                qreal y = (j + shiftY) / InitScale;
                qreal pixelValue = qBound(0, static_cast<int>(_noiseGenerator.noise(x, y) * 255), 255);
                resImg.setPixel(i, j, qRgba(0, pixelValue, 0, 255));
            }
        }
    };

    QThread* thr1 = QThread::create(std::bind(noiseLambda, 0, 0, _size/2, _size/2));
    thr1->start();
    QThread* thr2 = QThread::create(std::bind(noiseLambda, 0, _size/2, _size/2, _size));
    thr2->start();
    QThread* thr3 = QThread::create(std::bind(noiseLambda, _size/2, 0, _size, _size/2));
    thr3->start();
    QThread* thr4 = QThread::create(std::bind(noiseLambda, _size/2, _size/2, _size, _size));
    thr4->start();

    thr1->wait();
    thr2->wait();
    thr3->wait();
    thr4->wait();

    emit noiseGenereted(resImg);
}

void NoiseEmulator::generateStaticNoiseMovement()
{
    _staticNoiseMove *= -1;
}

void NoiseEmulator::setGeneratedNoise(QImage img)
{
    _noiseImg = img;
    _currCenter = _nextCenter;
}
