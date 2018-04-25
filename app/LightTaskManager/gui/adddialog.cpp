#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(TaskManager &taskManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog),
    m_taskManager(taskManager),
    m_date(QDate())
{
    ui->setupUi(this);
    setup();
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::setup()
{
    ui->tagComboBox->clear();
    m_tags = m_taskManager.readTags();
    ui->tagComboBox->addItems(m_tags);

    ui->userComboBox->clear();
    m_users = m_taskManager.readUsers();
    ui->userComboBox->addItems(m_users);
}

void AddDialog::on_addButtonBox_accepted()
{
    if(ui->addTextEdit->toPlainText() == nullptr)
    {
        QMessageBox(QMessageBox::Information, "Information", "Введите описание задачи").exec();
    }
    else
    {
        QString desctiption = ui->addTextEdit->toPlainText();

        QString tags = ui->activeTagsLineEdit->text();
        /*QStringList allActiveTags = ui->activeTagsLineEdit->text().split(" ", QString::SkipEmptyParts);
        for(auto activeTag : allActiveTags)
        {
            tags += QStringLiteral("+") + activeTag + QStringLiteral(" ");
        }*/

        QString users = ui->activeUsersLineEdit->text();
        /*QStringList allActiveUsers = ui->activeUsersLineEdit->text().split(" ", QString::SkipEmptyParts);
        for(auto activeUser : allActiveUsers)
        {
            users += QStringLiteral("@") + activeUser + QStringLiteral(" ");
        }*/

        QString date = "until [" + m_date.toString() +"]";

        QString task = desctiption + " " + tags + users + " " + date;
        qDebug() << "new task =" << task;
        emit addTask(task);
    }
}

void AddDialog::on_calendarWidget_clicked(const QDate &date)
{
    m_date = date;
}

void AddDialog::on_addTagPushButton_clicked()
{
    QString tag = ui->tagComboBox->currentText();
    QString currentTags = ui->activeTagsLineEdit->text();
    ui->activeTagsLineEdit->setText(currentTags + QStringLiteral(" +") + tag);
}

void AddDialog::on_addNewTagPushButton_clicked()
{
    QString tag = ui->newTagLineEdit->text();
    QString currentTags = ui->activeTagsLineEdit->text();
    ui->activeTagsLineEdit->setText(currentTags + QStringLiteral(" +") + tag);

    int tagsCount = m_tags.size() + 1;
    m_taskManager.getSettingsManager().set("Tags", "Count", tagsCount);
    QString newTagName = QStringLiteral("Tag") + QString::number(tagsCount - 1);
    m_taskManager.getSettingsManager().set("Tags", newTagName, tag);
    m_taskManager.getSettingsManager().saveSettings();

    ui->tagComboBox->clear();
    ui->newTagLineEdit->clear();
    m_tags = m_taskManager.readTags();
    ui->tagComboBox->addItems(m_tags);
}

void AddDialog::on_addUserPushButton_clicked()
{
    QString user = ui->userComboBox->currentText();
    QString currentUsers = ui->activeUsersLineEdit->text();
    ui->activeUsersLineEdit->setText(currentUsers + QStringLiteral(" @") + user);
}

void AddDialog::on_addNewUserPushButton_clicked()
{
    QString user = ui->newUserLineEdit->text();
    QString currentUsers = ui->activeUsersLineEdit->text();
    ui->activeUsersLineEdit->setText(currentUsers + QStringLiteral(" @") + user);

    int usersCount = m_users.size() + 1;
    m_taskManager.getSettingsManager().set("Users", "Count", usersCount);
    QString newUserName = QStringLiteral("User") + QString::number(usersCount - 1);
    m_taskManager.getSettingsManager().set("Users", newUserName, user);
    m_taskManager.getSettingsManager().saveSettings();

    ui->userComboBox->clear();
    ui->newUserLineEdit->clear();
    m_users = m_taskManager.readUsers();
    ui->userComboBox->addItems(m_users);
}
