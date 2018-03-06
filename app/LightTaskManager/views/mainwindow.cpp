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
    ui->mainToolBar->addAction(ui->actionInitialize);
    ui->mainToolBar->addAction(ui->actionAddTask);
    ui->mainToolBar->addAction(ui->actionDeleteTask);

    ui->statusBar->setStyleSheet("background-color:#333; color: #55bb55");
    ui->statusBar->showMessage("Ready");
}

void MainWindow::setupPresenter()
{

}
