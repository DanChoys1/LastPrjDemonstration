#include <QApplication>

#include "controllerlib/basecontroller.h"
#include "db_connection.h"

#include "rls_manager.h"
#include "setting_app_style.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Задаем стили приложения
    setAppStyle();

    // Подулючение к БД
    DBConnection::connectToSituation(); // Таблица целей
    DBConnection::connectToSyncPSql();  // Таблица носителя

    // Запуск РЛС
    RlsManager rls;
    // Задаем название приложения
    static const QString AppName { QStringLiteral("RLS-MR231") };
    rls.setObjectName(AppName);
    qApp->setApplicationName(AppName);

    // Создаем объект контроллера
    BaseController controller;
    controller.addForm(rls.rlsWindow());        // Добавляем окно РЛС
    controller.addForm(rls.virtualKeyboard());  // Добавляем виртуальную клавиатуру

    return a.exec();
}
