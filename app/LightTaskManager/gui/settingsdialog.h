#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>

#include "models/settingsmanager.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

signals:
    void applytodoDirectory(QString directory);

public:
    explicit SettingsDialog(SettingsManager settingsManager = SettingsManager(), QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_buttonBox_accepted();

    void on_todolistBinPathToolButton_clicked();

private:
    Ui::SettingsDialog *ui;
    SettingsManager m_settingsManager;

    void setup();
};

#endif // SETTINGSDIALOG_H
