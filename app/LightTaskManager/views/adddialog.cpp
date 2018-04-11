#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog),
    m_date(QDate())
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
        QString task =  ui->addTextEdit->toPlainText() + " " +
                "+" + ui->projectLineEdit->text() + " " +
                " until [" + m_date.toString() +"]" +
                " @" + ui->personLineEdit->text();
        qDebug() << "new task =" << task;
        emit addTask(task);
    }
}

void AddDialog::on_calendarWidget_clicked(const QDate &date)
{
    m_date = date;
}
