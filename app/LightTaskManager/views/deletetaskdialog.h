#ifndef DELETETASKDIALOG_H
#define DELETETASKDIALOG_H

#include <QDialog>

namespace Ui {
class DeleteTaskDialog;
}

class DeleteTaskDialog : public QDialog
{
    Q_OBJECT

signals:
    void deleteTask(QString index);

public:
    explicit DeleteTaskDialog(QWidget *parent = 0);
    ~DeleteTaskDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DeleteTaskDialog *ui;
};

#endif // DELETETASKDIALOG_H
