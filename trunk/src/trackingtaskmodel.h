#ifndef TRACKINGTASKMODEL_H
#define TRACKINGTASKMODEL_H

#include "taskmodel.h"

class TimeWatcher;

class TrackingTaskModel : public TaskModel
{
    Q_OBJECT
    public:
        static TrackingTaskModel *getSharedModel();

    private:
        explicit TrackingTaskModel(QObject *parent = 0);

        QString lastBackupPath_;
        TimeWatcher *timeWatcher;

    private slots:
        void refresh();
        void checkDeadlines();
};

#endif // TRACKINGTASKMODEL_H
