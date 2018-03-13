#include "mainwindowpresenter.h"

MainWindowPresenter::MainWindowPresenter(QObject *parent) :
    QObject(parent),
    m_todolistAdapter(new TodolistAdapter(this))
{
    connect(m_todolistAdapter, SIGNAL(directoryUpdated(QString)), this, SLOT(readDirectory(QString)));
}

MainWindowPresenter::~MainWindowPresenter()
{
    delete m_todolistAdapter;
}

void MainWindowPresenter::openRepository(QString directory)
{
    m_todolistAdapter->openRepository(directory);
}

void MainWindowPresenter::readDirectory(QString directory)
{
    emit directoryUpdated(directory);
}
