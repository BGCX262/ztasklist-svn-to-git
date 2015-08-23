#ifndef TASKFILTERMODE_H
#define TASKFILTERMODE_H

#include <QSortFilterProxyModel>
#include "task.h"

class TaskModel;

class TaskFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    enum privacyFilterMode{all,onlyPublic,onlyPrivate};
    TaskFilterModel(QObject * parent = 0 );

    bool completedHide();
    void setCompletedHide(bool);
    void setNonTodayHide(bool);
    void setPrivacyFilterMode(privacyFilterMode);
    void setSourceModel(TaskModel*);
    void setNameFilter(QString);

signals:
    void taskListChanged(QList<Task>);

private:
    bool completedHidden;
    bool nonTodayHide_;
    privacyFilterMode filterState_;
    QString filterString_;

    TaskModel *sourceModel() const;
    bool filterAcceptsRow(int, const QModelIndex &) const;



};

#endif // TASKFILTERMODE_H
