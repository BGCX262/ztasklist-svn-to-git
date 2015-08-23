#include "taskcontextmenu.h"
#include "iconwrapper.h"

#include "programsettings.h"

TaskContextMenu::TaskContextMenu(QWidget *parent) :
    QMenu(parent)
{
    actionComplete=new QAction(IconWrapper::completeIcon(),tr("Toggle completion"),this);
    this->addAction(actionComplete);

    actionPrivate=new QAction(IconWrapper::privateIcon(),tr("Toggle private"),this);
    this->addAction(actionPrivate);
    actionPrivate->setVisible(ProgramSettings::settings()->privateEnable());
    connect(ProgramSettings::settings(),SIGNAL(privateTasksToggled(bool)),actionPrivate,SLOT(setVisible(bool)));

    this->addSeparator();

    actionHigh=new QAction(QIcon(":/priority/high"),tr("High priority"),this);
    this->addAction(actionHigh);


    actionNormal=new QAction(QIcon(":/priority/normal"),tr("Normal priority"),this);
    this->addAction(actionNormal);

    actionLow=new QAction(QIcon(":/priority/low"),tr("Low priority"),this);
    this->addAction(actionLow);

    this->addSeparator();

    actionNoRepeat=new QAction(QIcon(":/icons/clock"),tr("No repeat"),this);
    this->addAction(actionNoRepeat);

    actionDayly=new QAction(QIcon(":/icons/clock"),tr("Repeat dayly"),this);
    this->addAction(actionDayly);

    actionWeekly=new QAction(QIcon(":/icons/clock"),tr("Repeat weekly"),this);
    this->addAction(actionWeekly);

    actionmonthly=new QAction(QIcon(":/icons/clock"),tr("Repeat monthly"),this);
    this->addAction(actionmonthly);

    this->addSeparator();

    actionRemove=new QAction(IconWrapper::removeIcon(),tr("Remove task"),this);
    this->addAction(actionRemove);

    connect(this,SIGNAL(triggered(QAction*)),this,SLOT(prioritySelected(QAction*)));

}

void TaskContextMenu::prioritySelected(QAction* a)
{
    if (a==actionComplete) emit toggleComplete();
    if (a==actionRemove) emit removeTask();
    if (a==actionHigh) emit priorityChanged(2);
    if (a==actionNormal) emit priorityChanged(1);
    if (a==actionLow) emit priorityChanged(0);
    if (a==actionPrivate) emit togglePrivate();

    if (a==actionNoRepeat) emit repeatChaged(Task::NoRepeat);
    if (a==actionDayly) emit repeatChaged(Task::DayRepeat);
    if (a==actionWeekly) emit repeatChaged(Task::WeekRepeat);
    if (a==actionmonthly) emit repeatChaged(Task::monthRepeat);
}
