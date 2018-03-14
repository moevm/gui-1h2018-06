#include "todolistadapter.h"

TodolistAdapter::TodolistAdapter(QObject *parent) :
    QObject(parent),
    m_directory(""),
    m_todolistProcess(new QProcess(this))
{
    connect(m_todolistProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onMessage()));
}

TodolistAdapter::~TodolistAdapter()
{
    delete m_todolistProcess;
}

QString TodolistAdapter::currentDirectory() const
{
    return m_directory;
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
    disconnect(m_todolistProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onTasks()));
    connect(m_todolistProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onMessage()));

    QString args = m_todolistBinPath + " " + m_addTask + " " + text;
    qDebug() << "add task" << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::onMessage()
{
    QByteArray message = m_todolistProcess->readAllStandardOutput();
    qDebug() << "read message" << QString::fromUtf8(message);
    emit directoryUpdated(m_directory);
    emit tasksUpdated(message);
}
