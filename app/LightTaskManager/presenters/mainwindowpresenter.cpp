#include "mainwindowpresenter.h"

MainWindowPresenter::MainWindowPresenter(QObject *parent) :
    QObject(parent),
    m_todolistAdapter(new TodolistAdapter(this))
{

}

MainWindowPresenter::~MainWindowPresenter()
{
    delete m_todolistAdapter;
}
