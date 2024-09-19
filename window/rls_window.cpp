#include "rls_window.h"
#include "ui_rls_window.h"
#include "uicolors.h"

#include <QDateTime>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>

using namespace DisplaySettings;

namespace
{

constexpr int UpdateTimeFr = 1e3;

const QString WarmingUpTimeFormat = QStringLiteral("mm : ss");
const QString CurrentTimeFormat = QStringLiteral("hh : mm : ss");
const QString CurrentDateFormat = QStringLiteral("dd.MM.yy");

}

RlsWindow::RlsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RlsWindow)
{
    ui->setupUi(this);

    initStaticWatchPicture();
    initSignalPicture();
    initDSign();

    initValuePercisions();
    clearTargetData();

    ui->displayCoureseType->setEnum({ { DisplayCourse::Course, QStringLiteral("КУРС") }, { DisplayCourse::North, QStringLiteral("СЕВЕР") } });
    ui->displayPosType->setEnum({ { DisplayPos::Relative, QStringLiteral("ОД") }, { DisplayPos::True, QStringLiteral("ИД") } });
    ui->lidLod->setEnum({ { DisplaySettings::MoveVectorType::LID, QStringLiteral("ЛИД")}, {DisplaySettings::MoveVectorType::LOD, QStringLiteral("ЛОД") } });

    ui->sign84Label->setEnable(false);
    ui->wgsLabel->setEnable(false);

    ui->loupeGreenLabel->setStyleSheet("background-color: " + UIColors::FrameBorder.name() + ";");
    ui->loupeContainer->hide();

    ui->amplificationSignal->setValue(100);
    ui->volni->setValue(30);

    QVBoxLayout* menuLayout = static_cast<QVBoxLayout*>(ui->menu_container->layout());
    menuLayout->insertWidget(0, &menu);

    connect(&_updateDateTimer, &QTimer::timeout, this, &RlsWindow::updateDate);
    _updateDateTimer.start(UpdateTimeFr);

    powerOn(false);
}

RlsWindow::~RlsWindow()
{
    delete ui;
}

void RlsWindow::powerOn(bool isOn)
{
    ui->centralwidget->setVisible(isOn);
}

void RlsWindow::warmingUpTime(qreal time)
{
    QTime lostTime(0,0,time,0);
    ui->warminUpTime->setText(lostTime.toString(WarmingUpTimeFormat));
}

void RlsWindow::startWarmingUp()
{
    ui->watchPicture->hide();
    ui->currTimeLabel->hide();
    ui->currTime->hide();

    ui->warminUpLabel->show();
    ui->warminUpTime->show();
}

void RlsWindow::stopWarmingUp()
{
    ui->warminUpLabel->hide();
    ui->warminUpTime->hide();

    ui->watchPicture->show();
    ui->currTimeLabel->show();
    ui->currTime->show();
}

void RlsWindow::setDisplay(QGraphicsScene *scene)
{
    ui->graphicsView->setScene(scene);
}

void RlsWindow::setDisplayDist(qreal dist)
{
    dist = CoordinateMath::metToMil(dist);

    qreal ringDist = dist / _distRingCount;

    // Если есть дробная часть, то отображать числа дробной части,
    // если дробной части нет, то отображать десятичный ноль
    int percision = ringDist - (int)ringDist >= 0.01 ? -1 : 1;
    ui->displayDist->setPercision(percision);
    ui->ringDist->setPercision(percision);

    // Изменение точности ВД и Дальности курсора при изменении масштаба
    int perc = dist < 12 ? 3 :
                           dist > 48 ? 1 :
                                       2;
    ui->cursorDist->setPercision(perc);
    ui->visorDist->setPercision(perc);

    ui->displayDist->setValue(dist);
    ui->ringDist->setValue(ringDist);
}

void RlsWindow::setDistRingsCount(int count)
{
    _distRingCount = count;
}

void RlsWindow::setSelfSpeed(qreal speed)
{
    _self.speed = speed;

    ui->selfSpeedSns->setValue(CoordinateMath::metSecToUzl(_self.speed));
    ui->selfSpeedGiro->setValue(std::max(CoordinateMath::metSecToUzl(_self.speed) - 0.1, 0.0));
}

void RlsWindow::setSelfCourse(qreal course)
{
     _self.course = course;

     ui->selfCourseSns->setValue(_self.course);
     ui->selfCourseGiro->setValue(std::max(_self.course - 0.3, 0.0));
}

void RlsWindow::setSelfPos(GeoPoint pos)
{
    _self.pos = pos;

    ui->selfLat->setText(latToString(_self.pos.lat));
    ui->selfLon->setText(lonToString(_self.pos.lon));

    setVisorPos(_visorPos); // Для обновления КУ
}

void RlsWindow::setVisorPos(const PolarPoint& pos)
{
    _visorPos = pos;

    ui->visorDist->setValue(CoordinateMath::metToMil(_visorPos.D));
    ui->visorBear->setValue(CoordinateMath::to360(_visorPos.B));

    qreal head = CoordinateMath::to180(_visorPos.B - _self.course);
    ui->visorHead->setValue(std::abs(head));

    QString side = head < 0 ? QStringLiteral("ЛБ") : QStringLiteral("ПБ");
    ui->visorBortSide->setText(side);
}

void RlsWindow::setCursorPos(const RectPoint& recPos)
{
    PolarPoint polarPos = CoordinateMath::rectToPolarY(recPos);
    ui->cursorBear->setValue(polarPos.B);
    ui->cursorDist->setValue(CoordinateMath::metToMil(polarPos.D));

    GeoPoint geoP = CoordinateMath::rectToGeo(recPos, _self.pos.lat);
    ui->cursorLat->setText(latToString(geoP.lat + _self.pos.lat));
    ui->cursorLon->setText(lonToString(geoP.lon + _self.pos.lon));
}

void RlsWindow::setDisplayCourseType(DisplaySettings::DisplayCourse type)
{
    ui->displayCoureseType->setValue(type);

    bool isShowVisorBear = type == DisplayCourse::North;
    ui->visorBearLabel->setVisible(isShowVisorBear);
    ui->visorBear->setVisible(isShowVisorBear);
    ui->visorBearSign->setVisible(isShowVisorBear);
}

void RlsWindow::setDisplayPosType(DisplaySettings::DisplayPos type)
{
    ui->displayPosType->setValue(type);
}

void RlsWindow::setMoveVecType(DisplaySettings::MoveVectorType type)
{
    ui->lidLod->setValue(type);

    QString lidLodColor = QString(QStringLiteral("color: %1;")).arg(type == MoveVectorType::LID ? UIColors::LidType.name() :
                                                                                                  UIColors::LodType.name());
    ui->lidLod->setStyleSheet(lidLodColor);
}

void RlsWindow::setMoveVecTime(qreal time)
{
    ui->moveVecTime->setValue(time/60);
}

void RlsWindow::setTargetData(const RlsData::Target& target)
{
    ui->tNum->setValue(target.num);
    ui->tBearing->setValue(target.bear);
    ui->tDist->setValue(CoordinateMath::metToMil(target.dist));
    ui->tCourse->setValue(target.course);
    ui->tSpeed->setValue(CoordinateMath::metSecToUzl(target.speed));
    ui->tDKS->setValue(CoordinateMath::metToMil(target.dks));
    ui->tVKS->setValue(target.vks / 60);
    ui->tDPK->setValue(CoordinateMath::metToMil(target.dpk));
    ui->tVPK->setValue(target.vpk / 60);
    ui->tLat->setText(latToString(target.geoPos.lat));
    ui->tLon->setText(lonToString(target.geoPos.lon));
}

void RlsWindow::clearTargetData()
{
    ui->tNum->    clear();
    ui->tBearing->clear();
    ui->tDist->   clear();
    ui->tCourse-> clear();
    ui->tSpeed->  clear();
    ui->tDKS->    clear();
    ui->tVKS->    clear();
    ui->tDPK->    clear();
    ui->tVPK->    clear();
    ui->tLat->    clear();
    ui->tLon->    clear();
}

void RlsWindow::setTargetCount(qreal count)
{
    ui->targetCount->setValue(count);
}

void RlsWindow::setDotsTracesDelayTime(qreal time)
{
    ui->dotTracesDelay->setText(time == 0 ? QStringLiteral("ОТКЛ") :
                                            QString::number(time/60));
}

void RlsWindow::switchTracesDots(DisplaySettings::DisplayTracesDots display)
{
    switch (display)
    {
    case DisplayTracesDots::Off:
    case DisplayTracesDots::TracesDots:
        ui->dotsTraces->setText(QStringLiteral("СЛЕДЫ/ТОЧКИ"));
        break;
    case DisplayTracesDots::Dots:
        ui->dotsTraces->setText(QStringLiteral("ТОЧКИ"));
        break;
    case DisplayTracesDots::Traces:
        ui->dotsTraces->setText(QStringLiteral("СЛЕДЫ"));
        break;
    }
}

void RlsWindow::setDangerDKS(qreal dks)
{
    ui->dangerDKS->setValue(CoordinateMath::metToMil(dks));
}

void RlsWindow::setDangerVKS(qreal vks)
{

    ui->dangerVKS->setValue(vks/60);
}

void RlsWindow::setLoupe(QGraphicsScene *grScene)
{
    ui->loupeGrView->setScene(grScene);
}

void RlsWindow::toggleLoupeVisible()
{
    bool isVisible = ui->loupeContainer->isVisible();
    if (isVisible)
        ui->loupeContainer->hide();
    else if (!menu.isVisible())
        ui->loupeContainer->show();
}

void RlsWindow::toggleMenuVisible()
{
    bool isVisible = menu.isVisible();
    if (isVisible)
        menu.hide();
    else if (!ui->loupeContainer->isVisible())
        menu.show();
}

QString RlsWindow::latToString(qreal lat) const
{
    QString side = lat < 0 ? QStringLiteral("S") : QStringLiteral("N");
    int deg = std::abs(static_cast<int>(lat));
    qreal minut = (std::abs(lat) - deg) * 60;
    return QString(QStringLiteral("%0°%1'%2"))
                                            .arg(deg, 2, 10, QLatin1Char('0'))
                                            .arg(minut, 6, 'f', 3, QLatin1Char('0'))
                                            .arg(side);
}

QString RlsWindow::lonToString(qreal lon) const
{
    QString side = lon < 0 ? QStringLiteral("W") : QStringLiteral("E");
    int deg = std::abs(static_cast<int>(lon));
    qreal minut = (std::abs(lon) - deg) * 60;
    return QString(QStringLiteral("%0°%1'%2"))
                            .arg(deg, 3, 10, QLatin1Char('0'))
                            .arg(minut, 6, 'f', 3, QLatin1Char('0'))
                            .arg(side);
}

void RlsWindow::initValuePercisions()
{
    ui->selfCourseGiro->setPercision(1);
    ui->selfSpeedGiro->setPercision(1);
    ui->selfCourseSns->setPercision(1);
    ui->selfSpeedSns->setPercision(1);

    ui->tBearing->setPercision(1);
    ui->tDist->setPercision(2);
    ui->tCourse->setPercision(1);
    ui->tSpeed->setPercision(1);
    ui->tDKS->setPercision(2);
    ui->tVKS->setPercision(2);
    ui->tDPK->setPercision(2);
    ui->tVPK->setPercision(2);

    ui->visorDist->setPercision(2);
    ui->visorBear->setPercision(1);
    ui->visorHead->setPercision(1);

    ui->cursorDist->setPercision(2);
    ui->cursorBear->setPercision(1);

    ui->dangerDKS->setPercision(2);
    ui->dangerVKS->setPercision(0);
}

void RlsWindow::initStaticWatchPicture()
{
    static constexpr qreal WatchSize = 6;
    QPainterPath watchPath;
    watchPath.addEllipse({ -WatchSize, -WatchSize, WatchSize*2, WatchSize*2 });
    watchPath.moveTo(0,0);
    watchPath.lineTo(-WatchSize, 0);
    watchPath.moveTo(0,0);
    watchPath.lineTo(CoordinateMath::polarToSceneY(WatchSize, 45));

    static constexpr qreal PixSize = WatchSize + 1;
    QPixmap pix(PixSize*2, PixSize*2);
    pix.fill(Qt::transparent);

    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing);
    p.translate(PixSize, PixSize);
    p.setPen(QPen(UIColors::StaticText, 2));

    p.drawPath(watchPath);

    ui->watchPicture->setPixmap(pix);
}

void RlsWindow::initSignalPicture()
{
    QPixmap signalPicture { ui->signalPicture->size() };
    signalPicture.fill(Qt::transparent);

    const qreal lineSize = signalPicture.width()  / 4;
    static constexpr qreal bottomPos = 20;
    static constexpr qreal topPos = 13;

    QPainter p(&signalPicture);
    p.setPen(QPen(UIColors::DynamicText, 2));

    p.drawLine(0,           bottomPos,  lineSize,   bottomPos);
    p.drawLine(lineSize,    bottomPos,  lineSize,   topPos);
    p.drawLine(lineSize,    topPos,     lineSize*2, topPos);
    p.drawLine(lineSize*2,  topPos,     lineSize*2, bottomPos);
    p.drawLine(lineSize*2,  bottomPos,  lineSize*4, bottomPos);

    ui->signalPicture->setPixmap(signalPicture);
}

void RlsWindow::initDSign()
{
    QPixmap dPix { ui->signalPicture->size() };
    dPix.fill(Qt::transparent);
    QPainter painter(&dPix);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(UIColors::StaticText, 3));
    painter.drawRoundRect(50,2,18,30-4,32,32);

    painter.setPen(QPen(Qt::black, 7));
    painter.drawLine(50,1,50,30-3);

    painter.setPen(QPen(UIColors::StaticText, 3));
    painter.drawLine(50+7,3,50+7,30-4);

    ui->DLabel->setPixmap(dPix);
}

void RlsWindow::updateDate()
{
    QDateTime curr = QDateTime::currentDateTime();
    ui->currTime->setText(curr.toString(CurrentTimeFormat));
    ui->currDate->setText(curr.toString(CurrentDateFormat));
}
