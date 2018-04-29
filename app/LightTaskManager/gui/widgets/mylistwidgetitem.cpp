#include "mylistwidgetitem.h"

MyListWidgetItem::MyListWidgetItem(QString index, QString description, QString date, QStringList tags, QStringList users, QWidget *parent) :
    QWidget(parent),
    m_index(index),
    m_description(description),
    m_date(date),
    m_tags(tags),
    m_users(users)
{
    QVBoxLayout* generalLayout = new QVBoxLayout(this);

    QFrame* frame = new QFrame(this);
    frame->setStyleSheet("background-color: #dfdfdf;");
    QVBoxLayout* frameLayout = new QVBoxLayout(frame);

    QHBoxLayout* titleLayout = new QHBoxLayout(frame);

    QLabel* indexLabel = new QLabel(m_index);
    titleLayout->addWidget(indexLabel);

    QLabel* descriptionLabel = new QLabel(m_description);
    titleLayout->addWidget(descriptionLabel);

    frameLayout->addLayout(titleLayout);

    QHBoxLayout* tagsLayout = new QHBoxLayout(this);
    for(auto tag : m_tags)
    {
        QLabel* label = new QLabel(tag);
        label->setStyleSheet("color: #55bb55;");
        tagsLayout->addWidget(label);
    }
    frameLayout->addLayout(tagsLayout);

    QHBoxLayout* usersLayout = new QHBoxLayout(this);
    for(auto user : m_users)
    {
        QLabel* label = new QLabel(user);
        label->setStyleSheet("color: #bb0000;");
        usersLayout->addWidget(label);
    }
    frameLayout->addLayout(usersLayout);

    QLabel* datelabel = new QLabel(m_date);
    generalLayout->addWidget(datelabel);

    generalLayout->addWidget(frame);
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
