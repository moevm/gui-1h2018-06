#ifndef TODOLISTADAPTER_H
#define TODOLISTADAPTER_H

#include <QObject>

class TodolistAdapter : public QObject
{
    Q_OBJECT
public:
    explicit TodolistAdapter(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TODOLISTADAPTER_H