#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_presenter(new MainWindowPresenter(this))
{
    ui->setupUi(this);
    setupWidgets();
    setupPresenter();
}

MainWindow::~MainWindow()
{
    delete m_presenter;
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

    ui->statusBar->setStyleSheet("background-color:#333; color: #55bb55");
    ui->statusBar->showMessage("Ready");

    viewToDo(QStringList());
}

void MainWindow::setupPresenter()
{
    connect(m_presenter, SIGNAL(directoryUpdated(QString)), this, SLOT(viewDirectory(QString)));
    connect(m_presenter, SIGNAL(dataUpdated(QStringList)), this, SLOT(viewToDo(QStringList)));
}

void MainWindow::viewDirectory(QString filePath)
{
    ui->filePathLineEdit->setText(filePath);
}

void MainWindow::viewToDo(QStringList todoList)
{
    QStringList statusLabels =
    {
        "Поставлены",
        "Выполнены"
    };

    qDebug() << "update table" << todoList.size();
    ui->taskContainerTableWidget->clear();
    ui->taskContainerTableWidget->setColumnCount(statusLabels.size());

    ui->taskContainerTableWidget->setHorizontalHeaderLabels(statusLabels);

    QStringList todoItems;
    QStringList completedItems;

    for(auto item : todoList)
    {
        if(item.contains("[ ]"))
        {
            todoItems.push_back(item);
        }
        else
        {
            if(item.contains("[x]"))
            {
                completedItems.push_back(item);
            }
            else
            {
                qDebug() << "undefined status" << item;
            }
        }
    }

    size_t maxCount = std::max(todoItems.size(), completedItems.size());
    ui->taskContainerTableWidget->setRowCount(maxCount);

    for(size_t i = 0; i < (size_t) todoItems.size(); i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem(todoItems[i]);
        ui->taskContainerTableWidget->setItem(i, 0, item);
    }

    for(size_t i = 0; i < (size_t) completedItems.size(); i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem(completedItems[i]);
        ui->taskContainerTableWidget->setItem(i, 1, item);
    }

    // to stretch table
    QTableWidget *field = ui->taskContainerTableWidget;
    for (int i = 0; i < field->columnCount(); i++)
    {
        field->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
}

void MainWindow::on_actionOpenRepository_triggered()
{
    QString path = QFileDialog::getExistingDirectory(0,"Open Directory", "");
    m_presenter->openRepository(path);
}

void MainWindow::on_taskContainerTableWidget_clicked(const QModelIndex &index)
{
    ui->currentTaskPlainTextEdit->clear();
    QString content = index.data().toString();
    ui->currentTaskPlainTextEdit->setPlainText(content);
}

void MainWindow::on_actionInitializeRepository_triggered()
{
    QString path = QFileDialog::getExistingDirectory(0,"Open Directory", "");
    m_presenter->initializeRepository(path);
}
