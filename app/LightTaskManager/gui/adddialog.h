#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QDate>
#include <QMessageBox>
#include <QDebug>

#include "models/taskmanager.h"

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(TaskManager& taskManager, QWidget *parent = 0);
    ~AddDialog();

signals:
    void addTask(QString newTask);

private slots:
    void on_addButtonBox_accepted();

    void on_calendarWidget_clicked(const QDate &date);

    void on_addTagPushButton_clicked();

    void on_addUserPushButton_clicked();

    void addNewTag(QString tag);
    void addNewUser(QString user);

private:
    Ui::AddDialog *ui;
    TaskManager& m_taskManager;

    QDate m_date;

    QStringList m_tags;
    QStringList m_users;

    void setup();
};

#endif // ADDDIALOG_H
