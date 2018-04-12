#include "mylistwidget.h"

MyListWidget::MyListWidget(QWidget *parent) :
    QListWidget(parent)
{

}

void MyListWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    QModelIndex selectedItemIndex = this->currentIndex();
    int keyPressed = keyEvent->key();

    switch (keyPressed) {
    case Qt::Key_Return:
    {
        keyReturnPressed(selectedItemIndex);
        break;
    }
    case Qt::Key_Up:
    {
        keyUpPressed(selectedItemIndex);
        break;
    }
    case Qt::Key_Down:
    {
        keyDownPressed(selectedItemIndex);
        break;
    }
    default:
    {
        break;
    }
    }
}

void MyListWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-item")) {
        event->accept();
        event->setDropAction(Qt::MoveAction);
        QListWidgetItem *item = new QListWidgetItem;
        QString name = event->mimeData()->data("application/x-item");
        item->setText(name);
        //item->setIcon(QIcon(":/images/iString")); //set path to image
        addItem(item);
        emit dropAction(name);
    } else
        event->ignore();
}

void MyListWidget::startDrag(Qt::DropActions supportedActions)
{
    QListWidgetItem* item = currentItem();
    QMimeData* mimeData = new QMimeData;
    QByteArray ba;
    ba = item->text().toUtf8();
    mimeData->setData("application/x-item", ba);
    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
        delete takeItem(row(item));
        //emit itemDroped();
    }
}

void MyListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-item"))
        event->accept();
    else
        event->ignore();
}

void MyListWidget::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->mimeData()->hasFormat("application/x-item") && e->source() != this) {
        e->setDropAction(Qt::MoveAction);
        e->accept();
    } else
        e->ignore();
}

/*Qt::DropAction MyListWidget::supportedDropActions()
{
    return Qt::MoveAction;
}*/

void MyListWidget::keyReturnPressed(QModelIndex selectedItemIndex)
{
    int currentItemRow = selectedItemIndex.row();
    if(currentItemRow >= 0 && currentItemRow < this->count())
    {
        emit clicked(selectedItemIndex);
    }
}

void MyListWidget::keyUpPressed(QModelIndex selectedItemIndex)
{
    int currentItemRow = selectedItemIndex.row();
    int rowsCount = this->count();
    if(currentItemRow > 0)
    {
        this->setCurrentRow(currentItemRow - 1);
    }
    else
    {
        if(currentItemRow == 0)
        {
            this->setCurrentRow(rowsCount - 1);
        }
        else
        {
            this->setCurrentRow(0);
        }
    }
}

void MyListWidget::keyDownPressed(QModelIndex selectedItemIndex)
{
    int currentItemRow = selectedItemIndex.row();
    int rowsCount = this->count();
    if(currentItemRow >= 0)
    {
        if(currentItemRow == rowsCount - 1)
        {
            this->setCurrentRow(0);
        }
        else
        {
            this->setCurrentRow(currentItemRow + 1);
        }
    }
    else
    {
        this->setCurrentRow(rowsCount - 1);
    }
}
