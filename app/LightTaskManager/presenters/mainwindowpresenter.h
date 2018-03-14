#ifndef MAINWINDOWPRESENTER_H
#define MAINWINDOWPRESENTER_H

#include <QObject>

#include "models/todolistadapter.h"

class MainWindowPresenter : public QObject
{
    Q_OBJECT
public:
    explicit MainWindowPresenter(QObject *parent = nullptr);
    ~MainWindowPresenter();

protected:
    TodolistAdapter *m_todolistAdapter;

signals:
    void directoryUpdated(QString directory);
    void dataUpdated(QStringList todoList);

public slots:
    void openRepository(QString directory);
    void readDirectory(QString directory);
    void parseData(QByteArray data);
};

#endif // MAINWINDOWPRESENTER_H
