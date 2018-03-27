#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QFileDialog>
#include <QListWidgetItem>

#include "presenters/mainwindowpresenter.h"
#include "adddialog.h"
#include "deletetaskdialog.h"
#include "edittaskdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateDirectoryWidgets(QString filePath);
    void updateTaskWidgets(QStringList todoList);

    void on_actionOpenRepository_triggered();

    void on_actionInitializeRepository_triggered();

    void on_todoListWidget_clicked(const QModelIndex &index);

    void on_completedListWidget_clicked(const QModelIndex &index);

    void completeTaskAction(QString data);
    void uncompleteTaskAction(QString data);

    void on_actionAddTask_triggered();

    void enableTasksActions();

    void on_actionDeleteTask_triggered();

    void on_editTaskPushButton_clicked();

private:
    Ui::MainWindow *ui;
    MainWindowPresenter *m_presenter;

    void setupWidgets();
    void setupPresenter();
};

#endif // MAINWINDOW_H
