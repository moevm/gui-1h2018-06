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
    const QString m_initializeRepository = "init";
    QString m_directory;
    QByteArray m_data;

    const QString m_todolistBinPath = "/opt/todolist/todolist";
    const QString m_getTasks = "list";
    const QString m_addTask = "add";
    const QString m_deleteTask = "delete";
    const QString m_updateTask = "update";

    QString m_directory;

    QProcess* m_todolistProcess;


signals:
    void tasksUpdated(QByteArray tasks);
    void directoryUpdated(QString path);
    void newMessage(QByteArray message);

public slots:
    void initializeRepository(QString directory);
    void openRepository(QString directory);
    void onTasks();
    void onMessage();
};

#endif // TODOLISTADAPTER_H
