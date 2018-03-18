#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = 0);
    ~AddDialog();

signals:
    void addTask(QString newTask);

private slots:
    void on_addButtonBox_accepted();

private:
    Ui::AddDialog *ui;
};

#endif // ADDDIALOG_H
