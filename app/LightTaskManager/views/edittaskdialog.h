#ifndef EDITTASKDIALOG_H
#define EDITTASKDIALOG_H

#include <QDialog>

namespace Ui {
class EditTaskDialog;
}

class EditTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTaskDialog(QString _index, QString _text, QWidget *parent = 0);
    ~EditTaskDialog();

signals:
    void editTask(QString index, QString task);

private slots:
    void view();

    void on_editButtonBox_accepted();

private:
    Ui::EditTaskDialog *ui;
    QString index;
    QString text;
};

#endif // EDITTASKDIALOG_H
