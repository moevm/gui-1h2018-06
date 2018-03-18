#include "mainwindowpresenter.h"

MainWindowPresenter::MainWindowPresenter(QObject *parent) :
    QObject(parent),  
    m_settingsManager(new SettingsManager())
{
    try
    {
        QString todolistPath = m_settingsManager->get("General", "TodoListBinPath").toString();
        m_todolistAdapter = new TodolistAdapter(todolistPath, this);

        connect(m_todolistAdapter, SIGNAL(directoryUpdated(QString)), this, SLOT(readDirectory(QString)));
        connect(m_todolistAdapter, SIGNAL(tasksUpdated(QByteArray)), this, SLOT(parseData(QByteArray)));
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Ошибка", e.what()).exec();
    }
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
        str.remove("\t"); //delete tabs
        str.remove("\n all\n");
        QStringList todoList = str.split(QRegExp("\n"), QString::SkipEmptyParts);
        emit dataUpdated(todoList);
    }
    else
    {
        openRepository(m_todolistAdapter->currentDirectory());
    }
}

void MainWindowPresenter::completeTask(QString data)
{
    QString index = data.split(" ").operator [](1);
    qDebug() << data.split(" ") << index;
    bool isCorrect;
    size_t number = index.toUInt(&isCorrect);
    if(isCorrect)
    {
        completeTask(number);
    }
    else
    {
        QMessageBox(QMessageBox::Warning, "Ошибка", "Невозможно сменить статус задачи №" + index).exec();
    }
}

void MainWindowPresenter::completeTask(size_t index)
{
    m_todolistAdapter->completeTask(index);
}

void MainWindowPresenter::uncompleteTask(size_t index)
{
    m_todolistAdapter->uncompleteTask(index);
}
