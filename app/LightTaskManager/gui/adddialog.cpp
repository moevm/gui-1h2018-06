#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(SettingsManager& settingsManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog),
    m_settingsManager(settingsManager),
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
    m_tags = readTags();
    ui->tagComboBox->addItems(m_tags);

    ui->userComboBox->clear();
    m_users = readUsers();
    ui->userComboBox->addItems(m_users);
}

QStringList AddDialog::readTags()
{
    QStringList tags;
    try
    {
        int tagsCount = m_settingsManager.get("Tags", "Count").toInt();
        if(tagsCount > 0)
        {
            for(size_t i = 0; i < (size_t) tagsCount; i++)
            {
                QString key = "Tag" + QString::number(i);
                QString tag = m_settingsManager.get("Tags", key).toString();
                tags.push_back(tag);
            }
        }
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Settings Error", e.what()).exec();
    }
    return tags;
}

QStringList AddDialog::readUsers()
{
    QStringList users;
    try
    {
        int usersCount = m_settingsManager.get("Users", "Count").toInt();
        if(usersCount > 0)
        {
            for(size_t i = 0; i < (size_t) usersCount; i++)
            {
                QString key = "User" + QString::number(i);
                QString user = m_settingsManager.get("Users", key).toString();
                users.push_back(user);
            }
        }
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Settings Error", e.what()).exec();
    }
    return users;
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
    m_settingsManager.set("Tags", "Count", tagsCount);
    QString newTagName = QStringLiteral("Tag") + QString::number(tagsCount - 1);
    m_settingsManager.set("Tags", newTagName, tag);
    m_settingsManager.saveSettings();

    ui->tagComboBox->clear();
    ui->newTagLineEdit->clear();
    m_tags = readTags();
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
    m_settingsManager.set("Users", "Count", usersCount);
    QString newUserName = QStringLiteral("User") + QString::number(usersCount - 1);
    m_settingsManager.set("Users", newUserName, user);
    m_settingsManager.saveSettings();

    ui->userComboBox->clear();
    ui->newUserLineEdit->clear();
    m_users = readUsers();
    ui->userComboBox->addItems(m_users);
}
