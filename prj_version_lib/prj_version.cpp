#include "prj_version.h"

#include <QFile>
#include <QTextStream>

QString prjVersion()
{
    Q_INIT_RESOURCE(prj_vers_lib_res);

    QFile inputFile(":/version/vers.txt");
    QString versionText = "";
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            versionText += in.readLine();
        }
        inputFile.close();
    }
    else
    {
        return inputFile.errorString();
    }

    return versionText;
}
