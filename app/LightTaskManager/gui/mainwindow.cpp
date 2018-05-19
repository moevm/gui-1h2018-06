#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_taskManager(new TaskManager(this))
{
    ui->setupUi(this);
    setupWidgets();
    setupModel();
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    qDeleteAll(m_statusesLabels.begin(), m_statusesLabels.end());
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

    ui->actionAddTask->setEnabled(false);
    ui->actionDeleteTask->setEnabled(false);
    ui->editTaskPushButton->setEnabled(false);
    ui->saveTaskPushButton->setEnabled(false);
    ui->acceptFiltersPushButton->setEnabled(false);
    ui->addTagToolButton->setEnabled(false);
    ui->addUserToolButton->setEnabled(false);
    ui->removeTagToolButton->setEnabled(false);
    ui->removeUserToolButton->setEnabled(false);

    updateTaskLists();
}

void MainWindow::setupModel()
{
    connect(m_taskManager.data(), SIGNAL(directoryUpdated(QString)), this, SLOT(updateDirectoryWidgets(QString)));
    connect(m_taskManager.data(), SIGNAL(dataUpdated(QStringList)), this, SLOT(setTasks(QStringList)));
    connect(m_taskManager.data(), SIGNAL(dataUpdated(QStringList)), this, SLOT(enableTasksActions()));
    connect(m_taskManager.data(), SIGNAL(statusMessage(QString)), this, SLOT(showStatusMessage(QString)));
}

void MainWindow::updateTaskLists()
{
    for(auto label : m_statusesLabels)
    {
        label->deleteLater();
    }

    for(auto list : m_tasksLists)
    {
        list->deleteLater();
    }

    m_statusesLabels.clear();
    m_tasksLists.clear();

    QStringList statuses = m_taskManager->readStatuses();

    for(auto status : statuses)
    {
        QLabel* statusLabel = new QLabel(ui->tasksContainerWidget);
        QString objectName = status + QStringLiteral("Label");
        statusLabel->setText(status);
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setObjectName(objectName);
        m_statusesLabels.push_back(statusLabel);

        MyListWidget* taskList = new MyListWidget(ui->tasksContainerWidget);
        objectName = status + QStringLiteral("ListWidget");
        taskList->setObjectName(objectName);
        taskList->setDragEnabled(true);
        taskList->setDropIndicatorShown(true);
        taskList->setDragDropMode(QAbstractItemView::DragDrop);
        connect(taskList, SIGNAL(dropAction(QString)), this, SLOT(changeTaskStatusAction(QString)));
        connect(taskList, SIGNAL(clicked(QModelIndex)), this, SLOT(showTask(QModelIndex)));
        m_tasksLists.push_back(taskList);
    }

    for(auto label : m_statusesLabels)
    {
        ui->statusesLabelsHorizontalLayout->addWidget(label);
    }

    for(auto list : m_tasksLists)
    {
        ui->tasksContainerHorizontalLayout->addWidget(list);
    }

    updateTaskWidgets();
}

void MainWindow::updateDirectoryWidgets(QString filePath)
{
    ui->filePathLineEdit->setText(filePath);
}

void MainWindow::setTasks(QStringList taskList)
{
    m_tasks = taskList;
    updateTaskWidgets();
}

void MainWindow::updateTaskWidgets()
{
    for(auto list : m_tasksLists)
    {
        list->clear();
    }

    ui->currentTaskIndexLineEdit->clear();
    ui->currentTaskTitleLineEdit->clear();
    ui->currentTaskTagsListWidget->clear();
    ui->currentTaskDateLineEdit->clear();
    ui->currentTaskUsersListWidget->clear();
    ui->currentTaskDescriptionPlainTextEdit->clear();

    QList< QStringList > tasksContainers;
    for(auto status : m_statusesLabels)
    {
        QStringList tmp;
        QString statusTemplate = QStringLiteral("[") + status->text() + QStringLiteral("]");
        for(auto item : m_tasks)
        {
            if(item.contains(statusTemplate))
            {
                QString data = item.remove(statusTemplate);
                tmp.push_back(data);
            }
        }
        tasksContainers.push_back(tmp);
    }

    for(size_t i = 0; i < (size_t) m_statusesLabels.size(); i++)
    {
        for(size_t j = 0; j < (size_t) tasksContainers[i].size(); j++)
        {
            QListWidgetItem* item = new QListWidgetItem();
            item->setTextAlignment(Qt::TopLeftCorner);
            item->setFont(QFont("Arial", -1, 10, false));

            QString data = tasksContainers[i][j];
            QString index = m_taskManager->getTaskIndex(data);
            QString title = m_taskManager->getTitle(data);
            QString desctiption = m_taskManager->getDescription(data);
            QString date = m_taskManager->getDate(data);
            QStringList tags = m_taskManager->getTags(data).split(" ", QString::SkipEmptyParts);
            QStringList users = m_taskManager->getUsers(data).split(" ", QString::SkipEmptyParts);

            item->setText(index);

            MyListWidgetItem* taskBoard = new MyListWidgetItem(index, title, desctiption, date, tags, users, m_tasksLists[i]);
            item->setSizeHint(taskBoard->minimumSizeHint());

            m_tasksLists[i]->addItem(item);
            m_tasksLists[i]->setItemWidget(item, taskBoard);
        }
    }
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
    // get tasks index
    QString index = data.split(" ", QString::SkipEmptyParts).at(0);

    // get task status
    QString status = "undefined";
    MyListWidget* senderWidget = qobject_cast<MyListWidget *>(sender());
    if(senderWidget)
    {

        for(size_t i = 0; i < (size_t) m_statusesLabels.size(); i++)
        {
            if(m_tasksLists[i] == senderWidget)
            {
                status = m_statusesLabels[i]->text();
            }
        }
    }

    // change task status by index
    m_taskManager->changeTaskStatus(index.toULongLong(), status);
}

void MainWindow::showTask(QModelIndex index)
{
    ui->currentTaskIndexLineEdit->clear();
    ui->currentTaskTitleLineEdit->clear();
    ui->currentTaskTagsListWidget->clear();
    ui->currentTaskDateLineEdit->clear();
    ui->currentTaskUsersListWidget->clear();
    ui->currentTaskDescriptionPlainTextEdit->clear();

    MyListWidget* list = qobject_cast<MyListWidget*>(sender());
    if(list)
    {
        MyListWidgetItem* item = qobject_cast<MyListWidgetItem*>(list->currentItem()->listWidget()->indexWidget(index));
        if(item)
        {
            ui->currentTaskIndexLineEdit->setText(item->index());
            ui->currentTaskTitleLineEdit->setText(item->title());
            ui->currentTaskTagsListWidget->addItems(item->tags().split(" ", QString::SkipEmptyParts));
            ui->currentTaskDateLineEdit->setText(item->date());
            ui->currentTaskUsersListWidget->addItems(item->users().split(" ", QString::SkipEmptyParts));
            ui->currentTaskDescriptionPlainTextEdit->setPlainText(item->description());
            ui->editTaskPushButton->setEnabled(true);
        }
        else
        {
            qDebug() << "can not convert QListWidgetItem to MyListWidgetItem";
        }
    }
    else
    {
        qDebug() << "Not success";
    }
}

void MainWindow::on_actionAddTask_triggered()
{
    AddDialog add(*(m_taskManager.data()), this);
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

void MainWindow::showStatusMessage(QString message)
{
    ui->statusBar->showMessage(message, 500);
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
    ui->currentTaskTitleLineEdit->setReadOnly(false);
    ui->currentTaskDescriptionPlainTextEdit->setReadOnly(false);
    ui->currentTaskDateLineEdit->setReadOnly(false);
    ui->saveTaskPushButton->setEnabled(true);
    ui->addTagToolButton->setEnabled(true);
    ui->addUserToolButton->setEnabled(true);
    ui->removeTagToolButton->setEnabled(true);
    ui->removeUserToolButton->setEnabled(true);

    for(size_t i = 0; i < (size_t) ui->currentTaskTagsListWidget->count(); i++)
    {
        QListWidgetItem* tagsListItem = ui->currentTaskTagsListWidget->item(i);
        tagsListItem->setFlags(tagsListItem->flags() | Qt::ItemIsEditable);
    }

    for(size_t i = 0; i < (size_t) ui->currentTaskUsersListWidget->count(); i++)
    {
        QListWidgetItem* usersListItem = ui->currentTaskUsersListWidget->item(i);
        usersListItem->setFlags(usersListItem->flags() | Qt::ItemIsEditable);
    }
}

void MainWindow::on_saveTaskPushButton_clicked()
{
    QString taskIndex = ui->currentTaskIndexLineEdit->text();

    QString taskTags = "";

    for(size_t i = 0; i < (size_t) ui->currentTaskTagsListWidget->count(); i++)
    {
        taskTags += QStringLiteral("+") + ui->currentTaskTagsListWidget->item(i)->text() + QStringLiteral(" ");
    }

    /*QStringList tags = ui->tagLineEdit->text().split(" ", QString::SkipEmptyParts);
    for(auto tag : tags)
    {
        taskTags += QStringLiteral("+") + tag + QStringLiteral(" ");
    }*/

    QString taskUsers = "";
    for(size_t i = 0; i < (size_t) ui->currentTaskUsersListWidget->count(); i++)
    {
        taskUsers += QStringLiteral("@") + ui->currentTaskUsersListWidget->item(i)->text() + QStringLiteral(" ");
    }

    /*QStringList users = ui->usersListWidget->;
    for(auto user : users)
    {
        taskUsers += QStringLiteral("@") + user + QStringLiteral(" ");
    }*/

    QString taskDate = QStringLiteral("until [") + ui->currentTaskDateLineEdit->text() + QStringLiteral("]");

    QString taskTitle = QStringLiteral("#") + ui->currentTaskTitleLineEdit->text() + QStringLiteral("#");
    QString taskDescription = ui->currentTaskDescriptionPlainTextEdit->toPlainText();

    QString task = taskTitle + " " + taskDescription + " " + taskTags + taskUsers + taskDate;

    m_taskManager->editTask(taskIndex, task);

    ui->currentTaskTitleLineEdit->setReadOnly(true);
    ui->currentTaskDescriptionPlainTextEdit->setReadOnly(true);
    ui->currentTaskDateLineEdit->setReadOnly(true);
    ui->saveTaskPushButton->setEnabled(false);
    ui->addTagToolButton->setEnabled(false);
    ui->addUserToolButton->setEnabled(false);
    ui->removeTagToolButton->setEnabled(false);
    ui->removeUserToolButton->setEnabled(false);
}

void MainWindow::on_actionOpenTerminal_triggered()
{
    m_taskManager->openTerminal(ui->filePathLineEdit->text());
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog dialog(m_taskManager->getSettingsManager(), this);
    connect(&dialog, SIGNAL(applytodoDirectory(QString)), m_taskManager.data(), SLOT(setTodolistDirectory(QString)));
    dialog.exec();
    disconnect(&dialog, SIGNAL(applytodoDirectory(QString)), m_taskManager.data(), SLOT(setTodolistDirectory(QString)));
    updateTaskLists();
}

void MainWindow::on_acceptFiltersPushButton_clicked()
{
    m_taskManager->setTagFilter(ui->filterByTagLineEdit->text());
    m_taskManager->setUserFilter(ui->filterByUserLineEdit->text());
    m_taskManager->reopenRepository();
}

void MainWindow::on_addTagToolButton_clicked()
{
    QListWidgetItem* emptyItem = new QListWidgetItem("");
    ui->currentTaskTagsListWidget->addItem(emptyItem);
    emptyItem->setFlags(emptyItem->flags() | Qt::ItemIsEditable);
    ui->currentTaskTagsListWidget->setCurrentItem(emptyItem);
}

void MainWindow::on_addUserToolButton_clicked()
{
    QListWidgetItem* emptyItem = new QListWidgetItem("");
    ui->currentTaskUsersListWidget->addItem(emptyItem);
    emptyItem->setFlags(emptyItem->flags() | Qt::ItemIsEditable);
    ui->currentTaskUsersListWidget->setCurrentItem(emptyItem);
}

void MainWindow::on_removeTagToolButton_clicked()
{
    qDeleteAll(ui->currentTaskTagsListWidget->selectedItems());
}

void MainWindow::on_removeUserToolButton_clicked()
{
    qDeleteAll(ui->currentTaskUsersListWidget->selectedItems());
}

void MainWindow::on_commandLineLineEdit_returnPressed()
{
    QString text = ui->commandLineLineEdit->text();
    if(text.length() > 0)
    {
        ui->commandLineLineEdit->clear();
        m_taskManager->runCommand(text);
    }
}
