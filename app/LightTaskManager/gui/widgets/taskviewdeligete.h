#ifndef TASKVIEWDELIGETE_H
#define TASKVIEWDELIGETE_H

#include <QItemDelegate>
#include <QApplication>
#include <QMainWindow>
#include <QPainter>
#include <QDebug>
#include <QPushButton>

class TaskViewDeligete : public QItemDelegate
{
    Q_OBJECT
public:
    explicit TaskViewDeligete(QWidget *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

protected:

signals:

public slots:
};

#endif // TASKVIEWDELIGETE_H
