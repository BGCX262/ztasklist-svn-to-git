#ifndef TASKLISTDELEGATE_H
#define TASKLISTDELEGATE_H

#include <QStyledItemDelegate>

class QToolButton;

class TaskDelegate : public QStyledItemDelegate
{
public:
    TaskDelegate(QObject *parent=0);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TASKLISTDELEGATE_H
