#include "taskmanager.h"

TaskManager::TaskManager(QObject *parent) :
    QObject(parent),  
    m_settingsManager(new SettingsManager()),
    m_tagFilter(""),
    m_userFilter("")
{
    qDebug() << "new instance of task Manager";
    try
    {
        QString todolistPath = m_settingsManager->get("General", "TodoListBinPath").toString();
        m_todolistAdapter = QSharedPointer<TodolistAdapter> (new TodolistAdapter(todolistPath, this));

        connect(m_todolistAdapter.data(), SIGNAL(directoryUpdated(QString)), this, SLOT(readDirectory(QString)));
        connect(m_todolistAdapter.data(), SIGNAL(tasksUpdated(QByteArray)), this, SLOT(parseData(QByteArray)));
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Ошибка", e.what()).exec();
    }
}

TaskManager::~TaskManager()
{

}

void TaskManager::setTagFilter(const QString &tagFilter)
{
    m_tagFilter = tagFilter;
}

void TaskManager::setUserFilter(const QString &userFilter)
{
    m_userFilter = userFilter;
}

void TaskManager::openRepository(QString directory)
{
    m_todolistAdapter->openRepository(directory);
}

void TaskManager::initializeRepository(QString directory)
{
    m_todolistAdapter->initializeRepository(directory);
}

void TaskManager::reopenRepository()
{
    openRepository(m_todolistAdapter->currentDirectory());
}

void TaskManager::readDirectory(QString directory)
{
    emit directoryUpdated(directory);
}

void TaskManager::parseData(QByteArray data)
{
    qDebug() << "filters:" << m_tagFilter << m_userFilter;

    QString str(data);
    if(str.contains("all"))
    {
        //QStringList todoList = str.split(QRegExp("[\n;\r]"), QString::SkipEmptyParts);
        str.remove("\t"); //delete tabs
        str.remove("\n all\n");
        QStringList todoList = str.split(QRegExp("\n"), QString::SkipEmptyParts);
        if(!m_tagFilter.isEmpty())
        {
            todoList = filterByTagName(todoList);
            qDebug() << "----- tasks filtered by tag -----" << m_tagFilter;
            qDebug() << todoList;
        }

        if(!m_userFilter.isEmpty())
        {
            todoList = filterByUserName(todoList);
            qDebug() << "----- tasks filtered by user -----" << m_userFilter;
            qDebug() << todoList;
        }

        emit dataUpdated(todoList);
    }
    else
    {
        reopenRepository();
    }
}

void TaskManager::completeTask(QString data)
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

void TaskManager::completeTask(size_t index)
{
    m_todolistAdapter->completeTask(index);
}

void TaskManager::uncompleteTask(QString data)
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

void TaskManager::uncompleteTask(size_t index)
{
    m_todolistAdapter->uncompleteTask(index);
}

void TaskManager::addTask(QString task)
{
    m_todolistAdapter->addTask(task);
}

void TaskManager::deleteTask(QString index)
{
    m_todolistAdapter->deleteTask(index.toUInt());
}

void TaskManager::editTask(QString index, QString task)
{
    m_todolistAdapter->editTask(index.toUInt(), task);
}

void TaskManager::openTerminal(QString path)
{
    qDebug() << "open terminal";
#ifdef Q_OS_MACOS
    QProcess::startDetached("open /Applications/Utilities/Terminal.app");
#endif
#ifdef Q_OS_LINUX
    QProcess::startDetached("konsole");
    QProcess::startDetached("gnome-terminal");
#endif

    //QProcess terminal(this);
    //m_term->setProgram("/bin/bash");
    //terminal.setWorkingDirectory(path);
    //QStringList args;
    //args << "-c ls";
    //args << "/bin/bash" <<"--rcfile <(echo '. ~/.bashrc; ls')";
    //m_term->setArguments(args);
    //m_term->setReadChannel(QProcess::StandardOutput);
    //m_term->setWorkingDirectory(path);
    //m_term->setCurrentReadChannel(QProcess::StandardOutput);
    //m_term->setCurrentWriteChannel(QProcess::StandardOutput);
    //m_term->start("konsole", QIODevice::ReadWrite);
    //qDebug() << m_term->waitForStarted(10000);
    //qDebug() << m_term->waitForReadyRead(10000);
    //qDebug() << m_term->isOpen();
    //qDebug() << m_term->isReadable() << m_term->isWritable();
    //m_term->write(QByteArray("asnd,nas"));
    //m_term->write("/bin/bash --rcfile <(echo '. ~/.bashrc; ls')");
    //qDebug() << m_term->writeChannelCount();
    //qDebug() << "read" << m_term->readAll() << m_term->readAllStandardError();
}

QString TaskManager::parseIndex(QString content)
{
    return content.section(" ", 1, 1);
}

QString TaskManager::parseTag(QString content)
{
    QString tag = "";
    if(content.contains("+"))
    {
        tag = content.split("+")[1].split(" ")[0];
    }
    return tag;
    //return content.section("+", 0, 0);
}

QString TaskManager::parseDate(QString content)
{
    QString date = "";
    if(content.contains("until ["))
    {
        date = content.split("until [")[1].split("]")[0];
        //qDebug() << "test reqExp" << date;
    }
    return date;
}

QString TaskManager::parseUser(QString content)
{
    QString res = "";
    if(content.contains("@"))
    {
        res = content.split("@").operator [](1).split(" ")[0];
    }
    return res;
    //return content.section("@", 1, 1);
}

QString TaskManager::parseTask(QString content)
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

void TaskManager::applytodoDirectory(QString directory)
{
    qDebug() << "todoDirectory" << directory;
    m_settingsManager->set("General", "TodoListBinPath", directory);
    m_settingsManager->saveSettings();

    QString todolistPath = m_settingsManager->get("General", "TodoListBinPath").toString();
    m_todolistAdapter->setBinPath(todolistPath);
}

QString TaskManager::todoSettingsPath()
{
    return m_todolistAdapter->currentTodoListBinPath();
}

SettingsManager &TaskManager::getSettingsManager()
{
    return (*(m_settingsManager.data()));
}

QStringList TaskManager::filterByTagName(QStringList allTasks)
{
    QString tagTemplate = QString(QString("+") + m_tagFilter);
    QStringList filteredTasks;
    for(auto task : allTasks)
    {
        QStringList splittedTask = task.split(" ");
        for(auto word : splittedTask)
        {
            if(word == tagTemplate)
            {
                filteredTasks.push_back(task);
            }
        }
    }
    return filteredTasks;
}

QStringList TaskManager::filterByUserName(QStringList allTasks)
{
    QStringList filteredTasks;
    QString userTemplate = QString(QString("@") + m_userFilter);
    for(auto task : allTasks)
    {
        QStringList splittedTask = task.split(" ");
        for(auto word : splittedTask)
        {
            if(word == userTemplate)
            {
                filteredTasks.push_back(task);
            }
        }
    }
    return filteredTasks;
}
