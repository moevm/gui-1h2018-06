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

public slots:
    void openRepository(QString directory);
    void initializeRepository(QString directory);
    void reopenRepository();

    void readDirectory(QString directory);
    void parseData(QByteArray data);    

    void changeTaskStatus(QString data, QString status);
    void changeTaskStatus(size_t index, QString status);

    void addTask(QString task);
    void deleteTask(QString index);
    void editTask(QString index, QString task);
    void openTerminal(QString path);
    QString parseIndex(QString content);
    QString getTitle(QString content);
    QString parseTag(QString content);
    QString parseDate(QString content);
    QString parseUser(QString content);
    QString parseTask(QString content);
    void applytodoDirectory(QString directory);
    QString todoSettingsPath();

    QStringList readStatuses();
    QStringList readTags();
    QStringList readUsers();

    SettingsManager& getSettingsManager();

protected slots:
    QStringList filterByTagName(QStringList allTasks);
    QStringList filterByUserName(QStringList allTasks);
};

#endif // TASKMANAGER_H
