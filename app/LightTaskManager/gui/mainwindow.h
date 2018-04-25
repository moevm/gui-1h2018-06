#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QLayout>

#include "models/taskmanager.h"
#include "gui/widgets/mylistwidget.h"

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
    void updateTaskWidgets(QStringList todoList);

    void on_actionOpenRepository_triggered();

    void on_actionInitializeRepository_triggered();

    void changeTaskStatusAction(QString data);

    void showTask(QModelIndex index);

    void on_actionAddTask_triggered();

    void enableTasksActions();

    void on_actionDeleteTask_triggered();

    void on_editTaskPushButton_clicked();

    void on_saveTaskPushButton_clicked();

    void on_actionOpenTerminal_triggered();

    void on_actionSettings_triggered();

    void on_acceptFiltersPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QScopedPointer<TaskManager> m_taskManager;
    QScopedPointer<SettingsManager> m_settingsManager;

    QStringList m_statuses;
    QList< MyListWidget* > m_tasksLists;

    void setupWidgets();
    void setupPresenter();
};

#endif // MAINWINDOW_H
