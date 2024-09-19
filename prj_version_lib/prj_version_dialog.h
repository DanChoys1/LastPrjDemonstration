#ifndef PRJVERSIONDIALOG_H
#define PRJVERSIONDIALOG_H

#include <QWidget>
#include <QShortcut>

class PrjVersionDialog : public QWidget
{
    Q_OBJECT
public:
    PrjVersionDialog(QWidget *p = nullptr);

    void setHotkey(const QKeySequence &key);

public slots:
    static void showDialog();

private:
    QShortcut _showDialogHotkey;

};

#endif // PRJVERSIONDIALOG_H
