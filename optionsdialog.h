#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QComboBox>

#include "ui_optionsdialog.h"

struct Options
{
    QString executable;
    QString server;
    QStringList args;
};

class OptionsDialog : public QDialog, private Ui_OptionsDialog
{
    Q_OBJECT

public:
    static Options getOptions(bool forStreamCommand);

    explicit OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();

public slots:
    void on_btnAppName_clicked();
    void on_cbBitRateAuto_clicked(bool checked);
    void done(int result);

private:
    void addComboItem(QComboBox* cb, const QString& title, const QString& value);
    void comboSelect(QComboBox* cb, const QString& value);
    QString comboGetValue(QComboBox* cb);
};

#endif // OPTIONSDIALOG_H
