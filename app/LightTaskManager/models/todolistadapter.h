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
    explicit TodolistAdapter(QString todolistBinPath, QObject *parent = nullptr);
    ~TodolistAdapter();

    QString currentDirectory() const;
    QString currentTodoListBinPath() const;

protected:
    const QString m_todolistBinPath = "";
    const QString m_initializeRepository = "init";
    const QString m_getTasks = "list";
    const QString m_addTask = "add";
    const QString m_deleteTask = "delete";
    const QString m_editTask = "edit";
    const QString m_completeTask = "complete";
    const QString m_uncompleteTask = "uncomplete";

    QString m_directory;
    QByteArray m_data;

    QProcess* m_todolistProcess;


signals:
    void tasksUpdated(QByteArray tasks);
    void directoryUpdated(QString path);
    void newMessage(QByteArray message);

public slots:
    void initializeRepository(QString directory);
    void openRepository(QString directory);
    void addTask(QString text);
    void completeTask(size_t index);
    void uncompleteTask(size_t index);
    void deleteTask(size_t index);
    void editTask(size_t index, QString text);

    void onMessage();
};

#endif // TODOLISTADAPTER_H
