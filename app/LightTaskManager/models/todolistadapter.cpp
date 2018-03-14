#include "todolistadapter.h"

TodolistAdapter::TodolistAdapter(QObject *parent) :
    QObject(parent),
    m_directory(""),
    m_todolistProcess(new QProcess(this))
{

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
    disconnect(m_todolistProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onTasks()));
    connect(m_todolistProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onMessage()));

    m_directory = directory;
    m_todolistProcess->setWorkingDirectory(m_directory);

    QString args = m_todolistBinPath + " " + m_initializeRepository;
    m_todolistProcess->start(args);
}

void TodolistAdapter::openRepository(QString directory)
{
    connect(m_todolistProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onTasks()));
    disconnect(m_todolistProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onMessage()));

    m_directory = directory;
    m_todolistProcess->setWorkingDirectory(m_directory);

    QString args = m_todolistBinPath + " " + m_getTasks;
    qDebug() << "open" << m_directory << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::onTasks()
{
    QByteArray tasks = m_todolistProcess->readAllStandardOutput();
    qDebug() << "read data" << QString::fromUtf8(tasks);
    emit directoryUpdated(m_directory);
    emit tasksUpdated(tasks);
    emit newMessage("");
}

void TodolistAdapter::onMessage()
{
    QByteArray message = m_todolistProcess->readAllStandardOutput();
    qDebug() << "read message" << QString::fromUtf8(message);
    emit directoryUpdated(m_directory);
    emit newMessage(message);
    emit tasksUpdated("");
}
