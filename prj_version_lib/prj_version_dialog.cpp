#include "prj_version_dialog.h"
#include "prj_version.h"

#include <QMessageBox>

namespace
{

constexpr QLatin1String _DialogHeader { "PrjVersion", 10 };

}

PrjVersionDialog::PrjVersionDialog(QWidget* p) :
    QWidget(p), _showDialogHotkey(this)
{
   connect(&_showDialogHotkey, &QShortcut::activated, this, &PrjVersionDialog::showDialog);
}

void PrjVersionDialog::setHotkey(const QKeySequence& key)
{
    _showDialogHotkey.setKey(key);
}

void PrjVersionDialog::showDialog()
{
    QMessageBox::information(nullptr, _DialogHeader, prjVersion());
}
