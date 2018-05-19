#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>

#include "models/todolistadapter.h"
#include "models/settingsmanager.h"

class TaskManager : public QObject
{
    Q_OBJECT

public:
    explicit TaskManager(QObject *parent = nullptr);
    ~TaskManager();

    /// setup filters
    void setTagFilter(const QString &tagFilter);
    void setUserFilter(const QString &userFilter);

protected:
    QScopedPointer<SettingsManager> m_settingsManager;
    QSharedPointer<TodolistAdapter> m_todolistAdapter;

    QString m_tagFilter;
    QString m_userFilter;

signals:
    void directoryUpdated(QString directory);
    void dataUpdated(QStringList todoList);
    void statusMessage(QString message);

public slots:

    /// working with adapter
    void runCommand(QString command);

    /// working with repositories
    void openRepository(QString directory);
    void initializeRepository(QString directory);
    void reopenRepository();

    /// update current directory
    void onCurrentDirectoryChanged(QString directory);

    /// handle todolist events
    void parseTodolistOutput(QByteArray data);

    /// working with tasks
    void changeTaskStatus(QString data, QString status);
    void changeTaskStatus(size_t index, QString status);
    void addTask(QString task);
    void deleteTask(QString index);
    void editTask(QString index, QString task);

    void openTerminal(QString path);

    /// get task information
    QString getTaskIndex(QString taskContent);
    QString getTitle(QString taskContent);
    QString getTags(QString taskContent);
    QString getDate(QString taskContent);
    QString getUsers(QString taskContent);
    QString getDescription(QString taskContent);

    /// setup todolist
    void setTodolistDirectory(QString directory);
    QString getTodolistDirectory();

    /// read information from config file
    QStringList readStatuses();
    QStringList readTags();
    QStringList readUsers();

    SettingsManager& getSettingsManager();

protected slots:

    /// filter tasks
    QStringList filterByTagName(QStringList allTasks);
    QStringList filterByUserName(QStringList allTasks);
};

#endif // TASKMANAGER_H
