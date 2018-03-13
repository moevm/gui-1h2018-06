#ifndef TODOLISTADAPTER_H
#define TODOLISTADAPTER_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QDebug>

class TodolistAdapter : public QObject
{
    Q_OBJECT
public:
    explicit TodolistAdapter(QObject *parent = nullptr);
    ~TodolistAdapter();

    QString currentDirectory() const;

protected:
    QString m_directory;
    QProcess* m_todolistProcess;

signals:
    void update(QByteArray* data);

public slots:
    void openRepository(QString directory);
    void readData();
};

#endif // TODOLISTADAPTER_H
