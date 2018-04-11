#include "todolistadapter.h"

TodolistAdapter::TodolistAdapter(QString todolistBinPath, QObject *parent) :
    QObject(parent),
    m_todolistBinPath(todolistBinPath),
    m_directory(""),
    m_todolistProcess(new QProcess(this))
{
    //connect(m_todolistProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onMessage()));
    connect(m_todolistProcess, SIGNAL(finished(int)), this, SLOT(onMessage()));
}

TodolistAdapter::~TodolistAdapter()
{
    delete m_todolistProcess;
}

QString TodolistAdapter::currentDirectory() const
{
    return m_directory;
}

QString TodolistAdapter::currentTodoListBinPath() const
{
    return m_todolistBinPath;
}

void TodolistAdapter::initializeRepository(QString directory)
{
    m_directory = directory;
    m_todolistProcess->setWorkingDirectory(m_directory);

    QString args = m_todolistBinPath + " " + m_initializeRepository;
    qDebug() << "initialize" << m_directory << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::openRepository(QString directory)
{
    m_directory = directory;
    m_todolistProcess->setWorkingDirectory(m_directory);

    QString args = m_todolistBinPath + " " + m_getTasks;
    qDebug() << "open" << m_directory << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::addTask(QString text)
{
    QString args = m_todolistBinPath + " " + m_addTask + " " + text;
    qDebug() << "add task" << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::completeTask(size_t index)
{
    QString args = m_todolistBinPath + " " + m_completeTask + " " + QString::number(index);
    qDebug() << "complete task" << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::uncompleteTask(size_t index)
{
    QString args = m_todolistBinPath + " " + m_uncompleteTask + " " + QString::number(index);
    qDebug() << "uncomplete task" << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::deleteTask(size_t index)
{
    QString args = m_todolistBinPath + " " + m_deleteTask + " " + QString::number(index);
    qDebug() << "delete task" << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::editTask(size_t index, QString text)
{
    QString args = m_todolistBinPath + " " + m_editTask + " " + QString::number(index) + " " + text;
    qDebug() << "edit task" << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::onMessage()
{
    QByteArray message = m_todolistProcess->readAllStandardOutput();
    qDebug() << "read message" << QString::fromUtf8(message);
    emit directoryUpdated(m_directory);
    emit tasksUpdated(message);
}
