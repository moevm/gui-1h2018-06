#include "mainwindowpresenter.h"

MainWindowPresenter::MainWindowPresenter(QObject *parent) :
    QObject(parent),
    m_todolistAdapter(new TodolistAdapter(this))
{
    connect(m_todolistAdapter, SIGNAL(directoryUpdated(QString)), this, SLOT(readDirectory(QString)));
    connect(m_todolistAdapter, SIGNAL(tasksUpdated(QByteArray)), this, SLOT(parseData(QByteArray)));
}

MainWindowPresenter::~MainWindowPresenter()
{
    delete m_todolistAdapter;
}

void MainWindowPresenter::openRepository(QString directory)
{
    m_todolistAdapter->openRepository(directory);
}

void MainWindowPresenter::initializeRepository(QString directory)
{
    m_todolistAdapter->initializeRepository(directory);
}

void MainWindowPresenter::readDirectory(QString directory)
{
    emit directoryUpdated(directory);
}

void MainWindowPresenter::parseData(QByteArray data)
{
    QString str(data);
    if(str.contains("all"))
    {
        //QStringList todoList = str.split(QRegExp("[\n;\r]"), QString::SkipEmptyParts);
        str.remove("\t"); //delete spaces
        str.remove("\n all\n");
        QStringList todoList = str.split(QRegExp("\n"), QString::SkipEmptyParts);
        emit dataUpdated(todoList);
    }
    else
    {
        openRepository(m_todolistAdapter->currentDirectory());
    }
}
