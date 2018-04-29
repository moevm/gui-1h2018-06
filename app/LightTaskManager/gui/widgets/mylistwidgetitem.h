#ifndef MYLISTWIDGETITEM_H
#define MYLISTWIDGETITEM_H

#include <QWidget>

class MyListWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit MyListWidgetItem(QWidget *parent = nullptr);

protected:
    QString m_title;
    QString m_description;
    QStringList m_tags;
    QStringList m_users;

signals:

public slots:
};

#endif // MYLISTWIDGETITEM_H
