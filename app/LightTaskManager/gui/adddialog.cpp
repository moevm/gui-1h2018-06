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
    if(ui->addTitleLineEdit->text() == nullptr)
    {
        QMessageBox(QMessageBox::Information, "Information", "Введите заголовок задачи").exec();
    }
    else
    {
        QString desctiption = QStringLiteral("#") + ui->addTitleLineEdit->text() + QStringLiteral("# ") +
                ui->addDescriptionTextEdit->toPlainText();

        QString tags = "";
        QStringList currentTags = ui->activeTagsLineEdit->text().split(" ", QString::SkipEmptyParts);
        QStringList supportedTags = m_taskManager.readTags();
        for(auto currentTag : currentTags)
        {
            QString currentTagData = currentTag;
            if(!(currentTagData.contains("+")))
            {
                currentTag = QStringLiteral("+") + currentTag;
            }
            else
            {
                currentTagData = currentTagData.remove("+");
            }

            bool contains = false;
            for(auto supportedTagData : supportedTags)
            {
                if(supportedTagData == currentTagData)
                {
                    contains = true;
                    break;
                }
            }

            if(!contains)
            {
                addNewTag(currentTagData);
            }

            tags += currentTag + " ";
        }


        QString users = "";
        QStringList currentUsers = ui->activeUsersLineEdit->text().split(" ", QString::SkipEmptyParts);
        QStringList supportedUsers = m_taskManager.readUsers();
        for(auto currentUser : currentUsers)
        {
            QString currentUserData = currentUser;
            if(!(currentUserData.contains("@")))
            {
                currentUser = QStringLiteral("@") + currentUser;
            }
            else
            {
                currentUserData = currentUserData.remove("@");
            }

            bool contains = false;
            for(auto supportedUserData : supportedUsers)
            {
                if(supportedUserData == currentUserData)
                {
                    contains = true;
                    break;
                }
            }

            if(!contains)
            {
                addNewUser(currentUserData);
            }

            users += currentUser + " ";
        }


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

void AddDialog::on_addUserPushButton_clicked()
{
    QString user = ui->userComboBox->currentText();
    QString currentUsers = ui->activeUsersLineEdit->text();
    ui->activeUsersLineEdit->setText(currentUsers + QStringLiteral(" @") + user);
}

void AddDialog::addNewTag(QString tag)
{
    int tagsCount = m_tags.size() + 1;
    m_taskManager.getSettingsManager().set("Tags", "Count", tagsCount);
    QString newTagName = QStringLiteral("Tag") + QString::number(tagsCount - 1);
    m_taskManager.getSettingsManager().set("Tags", newTagName, tag);
    m_taskManager.getSettingsManager().saveSettings();
}

void AddDialog::addNewUser(QString user)
{
    int usersCount = m_users.size() + 1;
    m_taskManager.getSettingsManager().set("Users", "Count", usersCount);
    QString newUserName = QStringLiteral("User") + QString::number(usersCount - 1);
    m_taskManager.getSettingsManager().set("Users", newUserName, user);
    m_taskManager.getSettingsManager().saveSettings();
}
