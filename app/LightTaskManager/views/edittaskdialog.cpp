#include "edittaskdialog.h"
#include "ui_edittaskdialog.h"

EditTaskDialog::EditTaskDialog(QString _index, QString _text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTaskDialog),
    index(_index),
    text(_text)
{
    ui->setupUi(this);
    view();
}

EditTaskDialog::~EditTaskDialog()
{
    delete ui;
}

void EditTaskDialog::view()
{
    ui->taskIndexLineEdit->setText(index);
    ui->editTextEdit->setText(text);
}

void EditTaskDialog::on_editButtonBox_accepted()
{
    emit editTask(ui->taskIndexLineEdit->text(), ui->editTextEdit->toPlainText());
}
