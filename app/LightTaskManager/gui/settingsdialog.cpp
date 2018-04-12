#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QString _path, QWidget *parent) :
    QDialog(parent),
    path(_path),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->settingsLineEdit->setText(path);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    emit applytodoDirectory(ui->settingsLineEdit->text());
    this->close();
}
