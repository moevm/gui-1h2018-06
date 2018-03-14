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
    ui->todoListWidget->clear();
    ui->completedListWidget->clear();

    QStringList todoItems;
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
    }
}

void MainWindow::on_actionOpenRepository_triggered()
{
    QString path = QFileDialog::getExistingDirectory(0,"Open Directory", "");
    m_presenter->openRepository(path);
}

void MainWindow::on_actionInitializeRepository_triggered()
{
    QString path = QFileDialog::getExistingDirectory(0,"Open Directory", "");
    m_presenter->initializeRepository(path);
}

void MainWindow::on_todoListWidget_clicked(const QModelIndex &index)
{
    ui->currentTaskPlainTextEdit->clear();
    QString content = index.data().toString();
    ui->currentTaskPlainTextEdit->setPlainText(content);
}

void MainWindow::on_completedListWidget_clicked(const QModelIndex &index)
{
    ui->currentTaskPlainTextEdit->clear();
    QString content = index.data().toString();
    ui->currentTaskPlainTextEdit->setPlainText(content);
}
