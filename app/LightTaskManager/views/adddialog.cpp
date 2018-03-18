#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::on_addButtonBox_accepted()
{
    if(ui->addTextEdit->toPlainText() == nullptr)
    {
        QMessageBox(QMessageBox::Information, "Information", "Введите описание задачи").exec();
    }
    else
    {
        QString task = ui->addTextEdit->toPlainText();
        emit addTask(task);
    }
}
