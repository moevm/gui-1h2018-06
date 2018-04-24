#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDebug>

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
    explicit SettingsDialog(const SettingsManager &settingsManager = SettingsManager(), QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SettingsDialog *ui;
    const SettingsManager& m_settingsManager;

    void setup();
};

#endif // SETTINGSDIALOG_H
