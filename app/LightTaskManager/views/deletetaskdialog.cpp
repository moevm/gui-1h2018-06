#include "deletetaskdialog.h"
#include "ui_deletetaskdialog.h"

DeleteTaskDialog::DeleteTaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteTaskDialog)
{
    ui->setupUi(this);
}

DeleteTaskDialog::~DeleteTaskDialog()
{
    delete ui;
}

void DeleteTaskDialog::on_buttonBox_accepted()
{
    emit deleteTask(ui->taskIndexLineEdit->text());
}
