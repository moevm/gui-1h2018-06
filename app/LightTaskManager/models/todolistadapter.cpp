#include "todolistadapter.h"

TodolistAdapter::TodolistAdapter(QObject *parent) :
    QObject(parent),
    m_directory(""),
    m_data(QByteArray()),
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

    QString args = m_todolistBinPath + " " + m_getTasks;
    m_todolistProcess->start(args);
    connect(m_todolistProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readData()));
}

void TodolistAdapter::readData()
{
    m_data = m_todolistProcess->readAllStandardOutput();
    qDebug() << "read" << QString::fromUtf8(m_data);
    emit dataUpdated(m_data);
    emit directoryUpdated(m_directory);
}
