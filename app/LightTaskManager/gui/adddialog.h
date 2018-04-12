#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QDate>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = 0);
    ~AddDialog();

signals:
    void addTask(QString newTask);

private slots:
    void on_addButtonBox_accepted();

    void on_calendarWidget_clicked(const QDate &date);

private:
    Ui::AddDialog *ui;
    QDate m_date;
};

#endif // ADDDIALOG_H
