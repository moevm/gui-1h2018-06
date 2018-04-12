#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QListWidget>
#include <QKeyEvent>
#include <QDropEvent>
#include <QDebug>
#include <QMimeData>
#include <QDrag>

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    MyListWidget(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *keyEvent);
    void dropEvent(QDropEvent *event);
    void startDrag(Qt::DropActions supportedActions);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* e);
    //Qt::DropAction supportedDropActions();
protected:
    void keyReturnPressed(QModelIndex selectedItemIndex);
    void keyUpPressed(QModelIndex selectedItemIndex);
    void keyDownPressed(QModelIndex selectedItemIndex);

signals:
    void dropAction(QString data);
};

#endif // MYLISTWIDGET_H
