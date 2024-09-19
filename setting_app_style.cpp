#include "setting_app_style.h"

#include "uicolors.h"

#include <QApplication>
#include <QFile>
#include <QFontDatabase>

/// Размер шрифта в РЛС
#ifdef Q_OS_WIN
const qreal     FontSize        = 12; //pt
#else
const qreal     FontSize        = 14; //pt
#endif

/// Толщина границ
const qreal     FrameBorderSize = 3; //px

/// Замена переменных в стиле на их значения
QString formatStyleSheet(QString styleStr)
{
    return styleStr
            .replace("FrameBorderSize",             QString::number(FrameBorderSize) + "px")
            .replace("FontSize",                    QString::number(FontSize) + "pt")
            .replace("StaticTextColor",             UIColors::StaticText.name())
            .replace("UnenabledTextColor",          UIColors::UnenabledText.name())
            .replace("DynamicTextColor",            UIColors::DynamicText.name())
            .replace("FrameBackgroundColor",        UIColors::FrameBack.name())
            .replace("FrameBorderColor",            UIColors::FrameBorder.name())
            .replace("MainWindowBackgroundColor",   UIColors::MWindowBack.name());
}

void setAppStyle()
{
    // Добавление шрифтка
    int id = QFontDatabase::addApplicationFont(QStringLiteral(":/fonts/Res/fonst/GOST.ttf"));
    QStringList familys = QFontDatabase::applicationFontFamilies(id);
    if (!familys.isEmpty())
        qApp->setFont(QFont{familys.first()});

    // Настройка внешнего вида приложения
    QFile file(QStringLiteral(":/styles/Res/style.qcc"));
    if (file.open(QIODevice::ReadOnly))
        qApp->setStyleSheet(formatStyleSheet(file.readAll()));
}
