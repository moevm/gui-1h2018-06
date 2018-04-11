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

void MainWindowPresenter::uncompleteTask(QString data)
{
    QString index = data.split(" ").operator [](1);
    qDebug() << data.split(" ") << index;
    bool isCorrect;
    size_t number = index.toUInt(&isCorrect);
    if(isCorrect)
    {
        uncompleteTask(number);
    }
    else
    {
        QMessageBox(QMessageBox::Warning, "Ошибка", "Невозможно сменить статус задачи №" + index).exec();
    }
}

void MainWindowPresenter::uncompleteTask(size_t index)
{
    m_todolistAdapter->uncompleteTask(index);
}

void MainWindowPresenter::addTask(QString task)
{
    m_todolistAdapter->addTask(task);
}

void MainWindowPresenter::deleteTask(QString index)
{
    m_todolistAdapter->deleteTask(index.toUInt());
}

void MainWindowPresenter::editTask(QString index, QString task)
{
    m_todolistAdapter->editTask(index.toUInt(), task);
}

void MainWindowPresenter::openTerminal(QString path)
{
    QProcess terminal;
    terminal.setWorkingDirectory(path);
    QStringList args;
    //args << "ls";
    qDebug() << "Open Terminal with args:" << args;
    qDebug() << terminal.startDetached("/bin/bash", args);
    //proc.startDetached("zenity --info --text Hello");
    //qDebug() << QProcess::startDetached("/bin/bash", args);   
}

QString MainWindowPresenter::parseIndex(QString content)
{
    return content.section(" ", 1, 1);
}

QString MainWindowPresenter::parseTag(QString content)
{
    QString tag = "";
    if(content.contains("+"))
    {
        tag = content.split("+")[1].split(" ")[0];
    }
    return tag;
    //return content.section("+", 0, 0);
}

QString MainWindowPresenter::parseDate(QString content)
{
    QString date = "";
    if(content.contains("until ["))
    {
        date = content.split("until [")[1].split("]")[0];
        //qDebug() << "test reqExp" << date;
    }
    return date;
}

QString MainWindowPresenter::parseUser(QString content)
{
    QString res = "";
    if(content.contains("@"))
    {
        res = content.split("@").operator [](1).split(" ")[0];
    }
    return res;
    //return content.section("@", 1, 1);
}

QString MainWindowPresenter::parseTask(QString content)
{
    QString res = "";
    QString index = content.split(" ", QString::SkipEmptyParts).operator[](0);
    content.remove(0, index.length() +1);
    //content = content.remove(parseIndex(content));
    content = content.remove("+" + parseTag(content));
    content = content.remove("@" + parseUser(content));
    content = content.remove(parseDate(content));
    content.remove("until [");
    content.remove("]");
    QStringList tmp = content.split(QRegExp(" "), QString::SkipEmptyParts);
    for(auto s : tmp)
    {
        res += s + QStringLiteral(" ");
    }
    return res;
}

void MainWindowPresenter::applytodoDirectory(QString directory)
{
    qDebug() << "todoDirectory" << directory;
    m_settingsManager->saveSettings();
    m_todolistAdapter->setBinPath(directory);
}

QString MainWindowPresenter::todoSettingsPath()
{
    return m_todolistAdapter->currentTodoListBinPath();
}
