#ifndef RLS_WINDOW_H
#define RLS_WINDOW_H

#include "ship_data.h"
#include "target_data.h"
#include "coordinate_math.h"
#include "menu_container.h"
#include "display_setting_enums.h"

#include <QGraphicsScene>
#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class RlsWindow; }
QT_END_NAMESPACE

class RlsWindow : public QWidget
{ Q_OBJECT

public:
    RlsWindow(QWidget *parent = nullptr);
    ~RlsWindow();

    void powerOn(bool isOn);
    void warmingUpTime(qreal time);
    void startWarmingUp();
    void stopWarmingUp();

    void setDisplay(QGraphicsScene* scene);

    void setDisplayDist(qreal dist);
    void setDistRingsCount(int count);

    void setSelfSpeed(qreal speed);
    void setSelfCourse(qreal course);
    void setSelfPos(GeoPoint pos);

    void setVisorPos(const PolarPoint &pos);
    void setCursorPos(const RectPoint &recPos);

    void setDisplayCourseType(DisplaySettings::DisplayCourse type);
    void setDisplayPosType(DisplaySettings::DisplayPos type);
    void setMoveVecType(DisplaySettings::MoveVectorType type);
    void setMoveVecTime(qreal time);

    void setTargetData(const RlsData::Target &target);
    void clearTargetData();
    void setTargetCount(qreal count);
    void setDotsTracesDelayTime(qreal time);
    void switchTracesDots(DisplaySettings::DisplayTracesDots display);

    void setDangerDKS(qreal dks);
    void setDangerVKS(qreal vks);

    void setLoupe(QGraphicsScene* grScene);
    void toggleLoupeVisible();

public slots:
    void toggleMenuVisible();

public:
    MenuContainer menu;

private:
    QString latToString(qreal lat) const;
    QString lonToString(qreal lon) const;

    void initValuePercisions();

    void initStaticWatchPicture();
    void initSignalPicture();
    void initDSign();

private slots:
    void updateDate();

private:
    Ui::RlsWindow *ui;

    RlsData::SelfShip _self;
    PolarPoint _visorPos;

    QTimer _updateDateTimer;

    int _distRingCount = 1;

};

#endif // RLS_WINDOW_H
