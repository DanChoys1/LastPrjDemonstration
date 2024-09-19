#ifndef INPUTCONTROL_H
#define INPUTCONTROL_H

#include <QWidget>

/// События ввода пользователем
class InputControl :  public QWidget
{ Q_OBJECT
public:
    InputControl(QWidget* p = nullptr) :
        QWidget(p) {}

    virtual ~InputControl() = default;

signals:
    /// Включение питания
    void powerOn(bool isOn);

    /// Увеличение масштаба
    void incScaleClicked();
    /// Уменьшение масштаба
    void decScaleClicked();

    /// Переключение развертки
    void toogleSearch();

    /// Переключение отображение колец дальности
    void toggleDistRingsVisibleClicked();
    /// Переключение отображение захваченных целей
    void toggleGrabTargetsVisibleClicked();

    /// Переключение отображения типа курса
    void toggleDisplayDirectTypeClicked();
    /// Переключение отображения типа позиции
    void toggleDisplayPosTypeClicked();
    /// Переключение отображения типа линий движения
    void toggleMoveVectorTypeClicked();

    /// Переключение отображения параллельный линий
    void toggleParallelLinesVisibleClicked();
    /// Переключение перемещения центра визора
    void switchTranslateVisor(bool isTranslate);

    /// Перемещение пеленга визора
    void visorBearTurned(qreal bear);
    /// Перемещение дистанции визора
    void visorDistTurned(qreal dist);
    /// Перемещение курсора
    void cursorMoved(qreal x, qreal y);

    /// Захват цели
    void grabTargetClicked();
    /// Сброс цели
    void ungrabTargetClicked();
    /// Выделение цели
    void selectTargetClicked();

    /// Переключение режимов отображения точки/следы
    void switchTracesDots();

    /// Переключение отображения лупы
    void toogleLoupeClicked();

    //////// MENU ////////
    /// Открыть меню
    void menuClicked();
    /// Ввод
    void startEditClicked();
    /// Следующее
    void nextMenuItemCliked();
    /// Предыдущее
    void prevMenuItemCliked();

};

#endif // INPUTCONTROL_H
