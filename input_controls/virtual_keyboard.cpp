#include "virtual_keyboard.h"
#include "ui_virtual_keyboard.h"

#include <QMouseEvent>

#include "polar_point.h"
#include "rect_point.h"
#include "coordinate_math.h"

VirtualKeyboard::VirtualKeyboard(QWidget *parent) :
    InputControl(parent),
    ui(new Ui::VirtualKeyboard)
{
    ui->setupUi(this);

    initDevices();
    connectDevises();
    connectSignals();

    ui->onOffRlsBulb->setLight(true);

    ui->cursorPanel->installEventFilter(this);

    cursorHidePixmap.fill(Qt::transparent);
}

VirtualKeyboard::~VirtualKeyboard()
{
    delete ui;
}

bool VirtualKeyboard::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mEv = static_cast<QMouseEvent*>(event);
        _cursorPressedPos  = mEv->globalPos();

        setCursor(QCursor(cursorHidePixmap));
    }
    else if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mEv = static_cast<QMouseEvent*>(event);

        RectPoint moveVec = CoordinateMath::toScene(mEv->globalPos() - _cursorPressedPos);
        emit cursorMoved(moveVec.x, moveVec.y);

//        QCursor::setPos(_cursorPressedPos);
        _cursorPressedPos = mEv->globalPos();
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }

    return true;
}

void VirtualKeyboard::initDevices()
{
    auto initBtns = [](Button *btn, const QString& path)
    {
        btn->addImage(path);
        btn->setCountStates(4);
    };

    initBtns(ui->reviewBtn,         ":/virtual_keyboard/Res/VirtualKeyboard/btn03.png");
    initBtns(ui->ungrabBtn,         ":/virtual_keyboard/Res/VirtualKeyboard/btn16.png");
    initBtns(ui->tracksDotsBtn,     ":/virtual_keyboard/Res/VirtualKeyboard/btn17.png");
    initBtns(ui->lidLodBtn,         ":/virtual_keyboard/Res/VirtualKeyboard/btn20.png");
    initBtns(ui->choiceTargetBtn,   ":/virtual_keyboard/Res/VirtualKeyboard/btn21.png");
    initBtns(ui->grabBtn,           ":/virtual_keyboard/Res/VirtualKeyboard/btn22.png");
    initBtns(ui->courseNorthBtn,    ":/virtual_keyboard/Res/VirtualKeyboard/btn05.png");
    initBtns(ui->idOdBtn,           ":/virtual_keyboard/Res/VirtualKeyboard/btn06.png");
    initBtns(ui->minus,             ":/virtual_keyboard/Res/VirtualKeyboard/btn11.png");
    initBtns(ui->plus,              ":/virtual_keyboard/Res/VirtualKeyboard/btn12.png");
    initBtns(ui->distRingsOnBtn,    ":/virtual_keyboard/Res/VirtualKeyboard/btn08.png");
    initBtns(ui->linePairBtn,       ":/virtual_keyboard/Res/VirtualKeyboard/btn10.png");
    initBtns(ui->offOkBtn,          ":/virtual_keyboard/Res/VirtualKeyboard/btn14.png");
    initBtns(ui->vnVdBtn,           ":/virtual_keyboard/Res/VirtualKeyboard/btn15.png");
    initBtns(ui->menuBtn,           ":/virtual_keyboard/Res/VirtualKeyboard/btn23.png");
    initBtns(ui->lupaBtn,           ":/virtual_keyboard/Res/VirtualKeyboard/btn24.png");

    ui->onOffRlsTumbler->addImage(":/virtual_keyboard/Res/VirtualKeyboard/toogle.png");

    ui->vnDial->addImage(":/virtual_keyboard/Res/VirtualKeyboard/rele1.png");
    ui->vdDial->addImage(":/virtual_keyboard/Res/VirtualKeyboard/rele1.png");

    ui->reviewBulb->addImage(":/virtual_keyboard/Res/VirtualKeyboard/lamp_yellow.png");
    ui->vnVdBulb->addImage(":/virtual_keyboard/Res/VirtualKeyboard/lamp_green2.png");
    ui->onOffRlsBulb->addImage(":/virtual_keyboard/Res/VirtualKeyboard/lamp_geen1.png");

    ui->widget_40       ->addImage(":/virtual_keyboard/Res/VirtualKeyboard/lamp_green2.png");
    ui->widget_41       ->addImage(":/virtual_keyboard/Res/VirtualKeyboard/lamp_green2.png");
    ui->widget_42       ->addImage(":/virtual_keyboard/Res/VirtualKeyboard/lamp_green2.png");
    ui->widget_44       ->addImage(":/virtual_keyboard/Res/VirtualKeyboard/lamp_green2.png");
    ui->widget_45       ->addImage(":/virtual_keyboard/Res/VirtualKeyboard/lamp_green2.png");
}

void VirtualKeyboard::connectDevises()
{
    connect(ui->reviewBtn, &Button::released, this, &VirtualKeyboard::switchReview);
    connect(ui->vnVdBtn, &Button::released, this, &VirtualKeyboard::switchVnVd);
}

void VirtualKeyboard::connectSignals()
{
    connect(ui->minus, &Button::released, this, &VirtualKeyboard::decScaleClicked);
    connect(ui->plus, &Button::released, this, &VirtualKeyboard::incScaleClicked);

    connect(ui->courseNorthBtn, &Button::released, this, &VirtualKeyboard::toggleDisplayDirectTypeClicked);
    connect(ui->idOdBtn, &Button::released, this, &VirtualKeyboard::toggleDisplayPosTypeClicked);

    connect(ui->vdDial, &Dial::rotationChangedTo, this, &VirtualKeyboard::visorDistTurned);
    connect(ui->vnDial, &Dial::rotationChangedTo, this, &VirtualKeyboard::visorBearTurned);

    connect(ui->distRingsOnBtn, &Button::released, this, &VirtualKeyboard::toggleDistRingsVisibleClicked);
    connect(ui->linePairBtn, &Button::released, this, &VirtualKeyboard::toggleParallelLinesVisibleClicked);
    connect(ui->vnVdBulb, &Bulb::lightChanged, this, &VirtualKeyboard::switchTranslateVisor);
    connect(ui->offOkBtn, &Button::pressed, this, &VirtualKeyboard::toggleGrabTargetsVisibleClicked);
    connect(ui->offOkBtn, &Button::released, this, &VirtualKeyboard::toggleGrabTargetsVisibleClicked);

    connect(ui->menuBtn, &Button::released, this, &VirtualKeyboard::menuClicked);
    connect(ui->menuBtn, &Button::released, this, &VirtualKeyboard::reconnectMenu);
    reconnectMenu();

    connect(ui->ungrabBtn, &Button::released, this, &VirtualKeyboard::ungrabTargetClicked);
    connect(ui->reviewBtn, &Button::released, this, &VirtualKeyboard::toogleSearch);

    connect(ui->tracksDotsBtn, &Button::released, this, &VirtualKeyboard::switchTracesDots);

    connect(ui->onOffRlsTumbler, &Tumbler::changeState, this, &VirtualKeyboard::powerOn);
    connect(ui->onOffRlsTumbler, &Tumbler::changeState, this, &VirtualKeyboard::powerChanged);

    connect(ui->lupaBtn, &Button::released, this, &VirtualKeyboard::toogleLoupeClicked);
}

void VirtualKeyboard::powerChanged(bool isOn)
{
    if (!isOn && ui->reviewBulb->isLight())
    {
        switchReview();
        emit toogleSearch();
    }
}

void VirtualKeyboard::switchReview()
{
    ui->reviewBulb->setLight(!ui->reviewBulb->isLight());
}

void VirtualKeyboard::switchVnVd()
{
    ui->vnVdBulb->setLight(!ui->vnVdBulb->isLight());
}

void VirtualKeyboard::reconnectMenu()
{
    if (!_isMenuOpen)
    {
        disconnect(ui->lidLodBtn, &Button::released, this, &VirtualKeyboard::toggleMoveVectorTypeClicked);
        disconnect(ui->choiceTargetBtn, &Button::released, this, &VirtualKeyboard::selectTargetClicked);
        disconnect(ui->grabBtn, &Button::released, this, &VirtualKeyboard::grabTargetClicked);

        connect(ui->lidLodBtn, &Button::released, this, &VirtualKeyboard::nextMenuItemCliked);
        connect(ui->choiceTargetBtn, &Button::released, this, &VirtualKeyboard::prevMenuItemCliked);
        connect(ui->grabBtn, &Button::released, this, &VirtualKeyboard::startEditClicked);

        _isMenuOpen = true;
    }
    else
    {
        disconnect(ui->lidLodBtn, &Button::released, this, &VirtualKeyboard::nextMenuItemCliked);
        disconnect(ui->choiceTargetBtn, &Button::released, this, &VirtualKeyboard::prevMenuItemCliked);
        disconnect(ui->grabBtn, &Button::released, this, &VirtualKeyboard::startEditClicked);

        connect(ui->lidLodBtn, &Button::released, this, &VirtualKeyboard::toggleMoveVectorTypeClicked);
        connect(ui->choiceTargetBtn, &Button::released, this, &VirtualKeyboard::selectTargetClicked);
        connect(ui->grabBtn, &Button::released, this, &VirtualKeyboard::grabTargetClicked);

        _isMenuOpen = false;
    }
}
