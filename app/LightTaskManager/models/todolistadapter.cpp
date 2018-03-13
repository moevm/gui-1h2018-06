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

void TodolistAdapter::openRepository(QString directory)
{
    m_directory = directory;

    qDebug() << "open";
    m_todolistProcess->setWorkingDirectory(m_directory);
    m_todolistProcess->start("/usr/local/opt/todolist/bin/todolist list");
    connect(m_todolistProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readData()));
}

void TodolistAdapter::readData()
{
    QByteArray data = m_todolistProcess->readAllStandardOutput();
    qDebug() << "read" << QString::fromUtf8(data);
    emit dataUpdated(data);
}
