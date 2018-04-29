#ifndef MYLISTWIDGETITEM_H
#define MYLISTWIDGETITEM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class MyListWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit MyListWidgetItem(QString index, QString description, QString date, QStringList tags, QStringList users, QWidget *parent = nullptr);

    QString index() const;

    QString description() const;

    QString tags() const;

    QString users() const;

    QString date() const;

protected:
    QString m_index;
    QString m_description;
    QString m_date;
    QStringList m_tags;
    QStringList m_users;

signals:

public slots:
};

#endif // MYLISTWIDGETITEM_H
