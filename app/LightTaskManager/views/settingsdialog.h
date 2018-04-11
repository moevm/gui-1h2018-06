#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

signals:
    void applytodoDirectory(QString directory);

public:
    explicit SettingsDialog(QString _path, QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SettingsDialog *ui;
    QString path;
};

#endif // SETTINGSDIALOG_H
