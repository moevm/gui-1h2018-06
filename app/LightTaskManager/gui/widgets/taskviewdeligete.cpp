#include "taskviewdeligete.h"

TaskViewDeligete::TaskViewDeligete(QWidget *parent) : QItemDelegate(parent)
{

}

void TaskViewDeligete::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->fillRect(option.rect.adjusted(0, 0, -1, -1), QBrush(QColor(250, 250, 250)));
    painter->drawText(option.rect.adjusted(0, 0, -1, -1), index.data().toString());
    qDebug() << "index data" << index.data().toString();


    /*if (index.column() != 2) {
        QItemDelegate::paint(painter, option, index);
        return;
    }*/

    // Set up a QStyleOptionProgressBar to precisely mimic the
    // environment of a progress bar.
    /*QStyleOptionProgressBar progressBarOption;
    progressBarOption.state = QStyle::State_Enabled;
    progressBarOption.direction = QApplication::layoutDirection();
    progressBarOption.rect = option.rect;
    progressBarOption.fontMetrics = QApplication::fontMetrics();
    progressBarOption.minimum = 0;
    progressBarOption.maximum = 100;
    progressBarOption.textAlignment = Qt::AlignCenter;
    progressBarOption.textVisible = true;*/

    // Set the progress and text values of the style option.
    /*int progress = qobject_cast<QMainWindow *>(parent())->clientForRow(index.row())->progress();
    progressBarOption.progress = progress < 0 ? 0 : progress;
    progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);*/

    // Draw the progress bar onto the view.
    //QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
}
