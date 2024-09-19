#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

namespace DBConnection
{
/// Подключение таблицы ситуации
void connectToSituation();
/// Подключение таблицы собственных данных (координаты, курс, ЦУ)
void connectToSyncPSql();
}

#endif // DB_CONNECTION_H
