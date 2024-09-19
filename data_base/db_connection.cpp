#include "db_connection.h"

#include "datamodel/syncpsql.h"
#include "dbhelper_situation/dbhelper_situation.h"

#include "self_ship.h"
#include "proton_maps.h"
#include "public_table.h"

//--host 192.168.101.15 --port 5432 --username stud2 --userpass 123 --dbname systema --table id1.VARS
void DBConnection::connectToSituation()
{
    //Получаем инстанс DbHelperSituation
    auto dbs = DbHelperSituation::instance();
    {
        // Создаем подключение. Реквизиты подключения будут взяты из ${application_dir}/psql.ini
        dbs->createConnect();
        // Подписываемся на канал обновления обстановки
        dbs->subscribe(DbHelperSituation::situation_props_channel);

        dbs->setHabitat(DbHelperSituation::SEA_AIR_LND_SUB::SEA);
        dbs->setColumns({
                            DbHelperSituation::COLUMNS_SITUATION::ID,
                            DbHelperSituation::COLUMNS_SITUATION::EPR_M2,
                            DbHelperSituation::COLUMNS_SITUATION::LATITUDE_DEG,
                            DbHelperSituation::COLUMNS_SITUATION::LONGITUDE_DEG,
                            DbHelperSituation::COLUMNS_SITUATION::HEADING_DEG,
                            DbHelperSituation::COLUMNS_SITUATION::SPEED_MS,
                            DbHelperSituation::COLUMNS_SITUATION::ALTITUDE_M
                        });
    }
}

void DBConnection::connectToSyncPSql()
{
    // Создем синхронизатор datamodel для PSQL
    static SyncPSql psqlSync;
    // Инициализируем его аргументами командной строки, переданными armclient
    psqlSync.initArgs();
    // Список входных данных
    QStringList inData =
    {
        DataHelpers::degpos,
        DataHelpers::head,
        DataHelpers::speed
    };
    // Список выходных данных
    QStringList outData =
    {
    };
    // Подписываемся на объединенный спискок
    psqlSync.subscribe(inData+outData);
    // Объявляем входные данные readonly во избежании неожиданностей
    psqlSync.setReadOnly(inData);
    // Чтение актуальных данных из БД
    psqlSync.readRemote();
}
