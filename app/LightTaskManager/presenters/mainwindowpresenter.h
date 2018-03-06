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

public slots:
};

#endif // MAINWINDOWPRESENTER_H
