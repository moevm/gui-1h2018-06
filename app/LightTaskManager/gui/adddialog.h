#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QDate>
#include <QMessageBox>
#include <QDebug>

#include "models/settingsmanager.h"

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(SettingsManager &settingsManager, QWidget *parent = 0);
    ~AddDialog();

signals:
    void addTask(QString newTask);

private slots:
    void on_addButtonBox_accepted();

    void on_calendarWidget_clicked(const QDate &date);

    void on_addTagPushButton_clicked();

    void on_addNewTagPushButton_clicked();

    void on_addUserPushButton_clicked();

    void on_addNewUserPushButton_clicked();

private:
    Ui::AddDialog *ui;
    SettingsManager& m_settingsManager;

    QDate m_date;

    QStringList m_tags;
    QStringList m_users;

    void setup();
    QStringList readTags();
    QStringList readUsers();
};

#endif // ADDDIALOG_H
