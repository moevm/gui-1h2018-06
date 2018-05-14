#include "mylistwidgetitem.h"

MyListWidgetItem::MyListWidgetItem(QString index, QString description, QString date, QStringList tags, QStringList users, QWidget *parent) :
    QWidget(parent),
    m_index(index),
    m_description(description),
    m_date(date),
    m_tags(tags),
    m_users(users)
{
    this->setStyleSheet("margin-left: 1px; margin-right: 1px; background-color: #eee; border: 1px solid #dfdfdf; border-radius: 5px;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFrame* container = new QFrame(this);
    //container->setStyleSheet("background-color: transparent;");
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    mainLayout->addWidget(container);

    //containerLayout->addWidget(new QLabel("#" + m_index, this));

    /*QTextEdit* descriptionTextEdit = new QTextEdit(this);
    descriptionTextEdit->setStyleSheet("font-weight: bold; border: 1px solid transparent; background-color: transparent;");
    descriptionTextEdit->setMaximumHeight(25);
    descriptionTextEdit->setPlainText(m_description);
    descriptionTextEdit->setReadOnly(true);
    descriptionTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    containerLayout->addWidget(descriptionTextEdit);*/

    QLabel* descriptionLabel = new QLabel(m_description, this);
    descriptionLabel->setStyleSheet("font-weight: bold; border: 1px solid transparent; background-color: transparent;");
    descriptionLabel->setMaximumWidth(container->width());
    containerLayout->addWidget(descriptionLabel);

    if(m_date.length() > 0)
    {
        QLabel* dateLabel = new QLabel("До: " + m_date, this);
        dateLabel->setStyleSheet("border: 1px solid transparent; background-color: transparent;");
        containerLayout->addWidget(dateLabel);
    }

    QHBoxLayout* paramsLayout = new QHBoxLayout(this);

    QVBoxLayout* tagsLayout = new QVBoxLayout(this);
    /*if(m_tags.count() > 0)
    {
        QLabel* tagsTitle = new QLabel("Тэги", this);
        tagsTitle->setStyleSheet("border: 1px solid transparent; background-color: transparent;");
        tagsTitle->setAlignment(Qt::AlignCenter);
        tagsLayout->addWidget(tagsTitle);
    }*/
    for(auto tag : m_tags)
    {
        QLabel* label = new QLabel(QStringLiteral("+") + tag, this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("background-color: #55bb55; color: #fff; border: 1px solid transparent; border-radius: 3px; padding: 1px;");
        tagsLayout->addWidget(label);
    }
    paramsLayout->addLayout(tagsLayout);

    QVBoxLayout* usersLayout = new QVBoxLayout(this);
    /*if(m_users.count() > 0)
    {
        QLabel* usersTitle = new QLabel("Пользовтели", this);
        usersTitle->setStyleSheet("border: 1px solid transparent; background-color: transparent;");
        usersTitle->setAlignment(Qt::AlignCenter);
        usersLayout->addWidget(usersTitle);
    }*/
    for(auto user : m_users)
    {
        QLabel* label = new QLabel(QStringLiteral("@") + user, this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("background-color: #4682b4; color: #fff; border: 1px solid transparent; border-radius: 3px; padding: 1px;");
        usersLayout->addWidget(label);
    }
    paramsLayout->addLayout(usersLayout);

    containerLayout->addLayout(paramsLayout);
}

QString MyListWidgetItem::index() const
{
    return m_index;
}

QString MyListWidgetItem::description() const
{
    return m_description;
}

QString MyListWidgetItem::tags() const
{
    QString tags;
    for(auto tag : m_tags)
    {
        tags += tag + " ";
    }
    return tags;
}

QString MyListWidgetItem::users() const
{
    QString users;
    for(auto user : m_users)
    {
        users += user + " ";
    }
    return users;
}

QString MyListWidgetItem::date() const
{
    return m_date;
}
