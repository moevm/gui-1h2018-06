#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QLayout>

#include "models/taskmanager.h"
#include "gui/widgets/mylistwidget.h"
#include "gui/widgets/mylistwidgetitem.h"

#include "adddialog.h"
#include "deletetaskdialog.h"
#include "settingsdialog.h"

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

    void setTasks(QStringList taskList);

    void updateTaskWidgets();

    void showTask(QModelIndex index);

    void enableTasksActions();

    void showStatusMessage(QString message);

    void on_actionOpenRepository_triggered();

    void on_actionInitializeRepository_triggered();

    void changeTaskStatusAction(QString data);

    void on_actionAddTask_triggered();

    void on_actionDeleteTask_triggered();

    void on_editTaskPushButton_clicked();

    void on_saveTaskPushButton_clicked();

    void on_actionOpenTerminal_triggered();

    void on_actionSettings_triggered();

    void on_acceptFiltersPushButton_clicked();

    void on_addTagToolButton_clicked();

    void on_addUserToolButton_clicked();

    void on_removeTagToolButton_clicked();

    void on_removeUserToolButton_clicked();

    void on_commandLineLineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    QScopedPointer<TaskManager> m_taskManager;

    QList< QLabel* > m_statusesLabels;
    QList< MyListWidget* > m_tasksLists;

    QStringList m_tasks;

    void setupWidgets();
    void setupModel();

    void updateTaskLists();
};

#endif // MAINWINDOW_H
