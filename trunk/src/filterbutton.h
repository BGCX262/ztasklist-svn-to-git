#ifndef FILTERBUTTON_H
#define FILTERBUTTON_H

#include <QToolButton>
#include "taskfiltermodel.h"

class FilterButton : public QToolButton
{
    Q_OBJECT
public:
    explicit FilterButton(QWidget *parent = 0);

private:
    QAction *allAction;
    QAction *publicAction;
    QAction *privateAction;

    QAction *onlyTodayAction;
    QAction *allDaysAction;

private slots:
    void actionTriggered(QAction *);

public slots:
    void setPrivateEnable(bool);

signals:
    void onlyTodayModeChanged(bool);
    void privacyModeChanged(TaskFilterModel::privacyFilterMode);

};

#endif // FILTERBUTTON_H
