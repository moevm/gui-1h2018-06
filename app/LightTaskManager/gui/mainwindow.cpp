#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_taskManager(new TaskManager(this)),
    m_settingsManager(new SettingsManager())
{
    ui->setupUi(this);
    setupWidgets();
    setupPresenter();
}

MainWindow::~MainWindow()
{
    qDeleteAll(m_tasksLists.begin(), m_tasksLists.end());
    delete ui;
}

void MainWindow::setupWidgets()
{
    auto toolbar = ui->mainToolBar;
    removeToolBar(toolbar);
    addToolBar(Qt::LeftToolBarArea, toolbar);
    toolbar->show();

    toolbar->addAction(ui->actionInitializeRepository);
    toolbar->addAction(ui->actionOpenRepository);
    toolbar->addAction(ui->actionAddTask);
    toolbar->addAction(ui->actionDeleteTask);
    toolbar->addAction(ui->actionSettings);

    ui->statusBar->setStyleSheet("background-color:#333; color: #55bb55");
    ui->statusBar->showMessage("Ready");

    //connect(ui->completedListWidget, SIGNAL(dropAction(QString)), this, SLOT(completeTaskAction(QString)));
    //connect(ui->todoListWidget, SIGNAL(dropAction(QString)), this, SLOT(uncompleteTaskAction(QString)));

    ui->actionAddTask->setEnabled(false);
    ui->actionDeleteTask->setEnabled(false);
    ui->editTaskPushButton->setEnabled(false);
    ui->saveTaskPushButton->setEnabled(false);
    ui->acceptFiltersPushButton->setEnabled(false);


    try
    {
        int statusesCount = m_settingsManager->get("Statuses", "Count").toInt();
        if(statusesCount > 0)
        {
            for(size_t i = 0; i < (size_t) statusesCount; i++)
            {
                QString key = QStringLiteral("Status") + QString::number(i);
                QString statusName = m_settingsManager->get("Statuses", key).toString();
                m_statuses.push_back(statusName);
            }
        }
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "SettingsError", e.what());
    }

    for(auto status : m_statuses)
    {
        MyListWidget* taskList = new MyListWidget(ui->tasksContainerWidget);
        QString objectName = status + QStringLiteral("ListWidget");
        taskList->setObjectName(objectName);
        connect(taskList, SIGNAL(dropAction(QString)), this, SLOT(changeTaskStatusAction(QString)));
        connect(taskList, SIGNAL(clicked(QModelIndex)), this, SLOT(showTask(QModelIndex)));
        ui->tasksContainerHorizontalLayout->addWidget(taskList);
        m_tasksLists.push_back(taskList);
    }

    updateTaskWidgets(QStringList());
}

void MainWindow::setupPresenter()
{
    connect(m_taskManager.data(), SIGNAL(directoryUpdated(QString)), this, SLOT(updateDirectoryWidgets(QString)));
    connect(m_taskManager.data(), SIGNAL(dataUpdated(QStringList)), this, SLOT(updateTaskWidgets(QStringList)));
    connect(m_taskManager.data(), SIGNAL(dataUpdated(QStringList)), this, SLOT(enableTasksActions()));
}

void MainWindow::updateDirectoryWidgets(QString filePath)
{
    ui->filePathLineEdit->setText(filePath);
}

void MainWindow::updateTaskWidgets(QStringList todoList)
{
    qDebug() << "----- tasks -----";
    qDebug() << todoList;

    ui->indexLineEdit->clear();
    ui->tagLineEdit->clear();
    ui->dateLineEdit->clear();
    ui->userLineEdit->clear();
    ui->currentTaskPlainTextEdit->clear();
    QList< QStringList > tasksContainers;
    for(auto status : m_statuses)
    {
        QStringList tmp;
        QString statusTemplate = QStringLiteral("[") + status + QStringLiteral("]");
        for(auto item : todoList)
        {
            if(item.contains(statusTemplate))
            {
                QString data = item.remove(statusTemplate);
                tmp.push_back(data);
            }
        }
        tasksContainers.push_back(tmp);
    }

    qDebug() << tasksContainers << m_statuses;

    for(size_t i = 0; i < (size_t) m_statuses.size(); i++)
    {
        m_tasksLists[i]->addItems(tasksContainers[i]);
    }

    /*QStringList todoItems;
    QStringList completedItems;

    for(auto item : todoList)
    {

        if(item.contains("[ ]"))
        {
            QString data = item.remove("[ ]");
            todoItems.push_back(data);
        }
        else
        {
            if(item.contains("[x]"))
            {
                QString data = item.remove("[x]");
                completedItems.push_back(data);
            }
            else
            {
                qDebug() << "undefined status" << item;
            }
        }
    }

    for(size_t i = 0; i < (size_t) todoItems.size(); i++)
    {
        QListWidgetItem* listItem = new QListWidgetItem(todoItems[i]);
        ui->todoListWidget->addItem(listItem);
    }

    for(size_t i = 0; i < (size_t) completedItems.size(); i++)
    {
        QListWidgetItem* listItem = new QListWidgetItem(completedItems[i]);
        ui->completedListWidget->addItem(listItem);
    }*/
}

void MainWindow::on_actionOpenRepository_triggered()
{
    QString path = QFileDialog::getExistingDirectory(0,"Open Directory", "");
    m_taskManager->openRepository(path);
}

void MainWindow::on_actionInitializeRepository_triggered()
{
    QString path = QFileDialog::getExistingDirectory(0,"Open Directory", "");
    m_taskManager->initializeRepository(path);
}

void MainWindow::changeTaskStatusAction(QString data)
{
    m_taskManager->changeTaskStatus(data);
}

void MainWindow::showTask(QModelIndex index)
{
    ui->currentTaskPlainTextEdit->clear();
    QString content = index.data().toString();
    ui->indexLineEdit->setText(m_taskManager->parseIndex(content));
    ui->tagLineEdit->setText(m_taskManager->parseTag(content));
    ui->dateLineEdit->setText(m_taskManager->parseDate(content));
    ui->userLineEdit->setText(m_taskManager->parseUser(content));
    ui->currentTaskPlainTextEdit->setPlainText(m_taskManager->parseTask(content));
    ui->editTaskPushButton->setEnabled(true);
}

void MainWindow::on_actionAddTask_triggered()
{
    AddDialog add(this);
    connect(&add, SIGNAL(addTask(QString)), m_taskManager.data(), SLOT(addTask(QString)));
    add.exec();
    disconnect(&add, SIGNAL(addTask(QString)), m_taskManager.data(), SLOT(addTask(QString)));
}

void MainWindow::enableTasksActions()
{
    ui->actionAddTask->setEnabled(true);
    ui->actionDeleteTask->setEnabled(true);
    ui->acceptFiltersPushButton->setEnabled(true);
}

void MainWindow::on_actionDeleteTask_triggered()
{
    DeleteTaskDialog dialog(this);
    connect(&dialog, SIGNAL(deleteTask(QString)), m_taskManager.data(), SLOT(deleteTask(QString)));
    dialog.exec();
    disconnect(&dialog, SIGNAL(deleteTask(QString)), m_taskManager.data(), SLOT(deleteTask(QString)));
}

void MainWindow::on_editTaskPushButton_clicked()
{
    ui->currentTaskPlainTextEdit->setReadOnly(false);
    ui->tagLineEdit->setReadOnly(false);
    ui->dateLineEdit->setReadOnly(false);
    ui->userLineEdit->setReadOnly(false);
    ui->saveTaskPushButton->setEnabled(true);
}

void MainWindow::on_saveTaskPushButton_clicked()
{
    QString index = ui->indexLineEdit->text();
    QString task = ui->currentTaskPlainTextEdit->toPlainText() + " " +
                   "+" + ui->tagLineEdit->text() + " " +
                   "until [" + ui->dateLineEdit->text() + "] " +
                   "@" + ui->userLineEdit->text();
    m_taskManager->editTask(index, task);

    ui->currentTaskPlainTextEdit->setReadOnly(true);
    ui->tagLineEdit->setReadOnly(true);
    ui->dateLineEdit->setReadOnly(true);
    ui->userLineEdit->setReadOnly(true);
    ui->saveTaskPushButton->setEnabled(false);
}

void MainWindow::on_actionOpenTerminal_triggered()
{
    m_taskManager->openTerminal(ui->filePathLineEdit->text());
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog dialog(m_taskManager->getSettingsManager(), this);
    connect(&dialog, SIGNAL(applytodoDirectory(QString)), m_taskManager.data(), SLOT(applytodoDirectory(QString)));
    dialog.exec();
    disconnect(&dialog, SIGNAL(applytodoDirectory(QString)), m_taskManager.data(), SLOT(applytodoDirectory(QString)));
}

void MainWindow::on_acceptFiltersPushButton_clicked()
{
    m_taskManager->setTagFilter(ui->filterByTagLineEdit->text());
    m_taskManager->setUserFilter(ui->filterByUserLineEdit->text());
    m_taskManager->reopenRepository();
}
