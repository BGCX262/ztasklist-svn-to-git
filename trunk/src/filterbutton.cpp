#include <QMenu>
#include "filterbutton.h"
#include "iconwrapper.h"

FilterButton::FilterButton(QWidget *parent) :
    QToolButton(parent)
{
    setIcon(IconWrapper::filterIcon());
    setPopupMode(QToolButton::InstantPopup);
    this->setCheckable(true);
    this->setToolTip(tr("Filters"));

    allAction=new QAction(tr("All tasks"),this);
    allAction->setCheckable(true);
    publicAction=new QAction(tr("Only public tasks"),this);
    publicAction->setCheckable(true);
    privateAction=new QAction(tr("Only private tasks"),this);
    privateAction->setCheckable(true);

    allDaysAction=new QAction(tr("All tasks"),this);
    allDaysAction->setCheckable(true);

    onlyTodayAction=new QAction(tr("Only today tasks"),this);
    onlyTodayAction->setCheckable(true);

    QActionGroup *todayGroup=new QActionGroup(this);
    todayGroup->addAction(allDaysAction);
    todayGroup->addAction(onlyTodayAction);
    allDaysAction->setChecked(true);

    QActionGroup *privateGroup=new QActionGroup(this);
    privateGroup->addAction(allAction);
    privateGroup->addAction(privateAction);
    privateGroup->addAction(publicAction);
    allAction->setChecked(true);

    QMenu *menu=new QMenu(this);
    menu->addAction(allDaysAction);
    menu->addAction(onlyTodayAction);
    menu->addSeparator()->setText(tr("Privacy"));
    menu->addAction(allAction);

    menu->addAction(privateAction);
    menu->addAction(publicAction);
    this->setMenu(menu);

    connect(menu,SIGNAL(triggered(QAction*)),this,SLOT(actionTriggered(QAction *)));
}

void FilterButton::setPrivateEnable(bool v)
{
    allAction->setVisible(v);
    publicAction->setVisible(v);
    privateAction->setVisible(v);
}

void FilterButton::actionTriggered(QAction *a)
{

    if (a==onlyTodayAction) {emit onlyTodayModeChanged(true); this->setChecked(true); return;}
    if (a==allDaysAction) {emit onlyTodayModeChanged(false); this->setChecked(false); return;}

    if (a==allAction) {emit privacyModeChanged(TaskFilterModel::all);this->setChecked(false); return;}
    if (a==privateAction) {emit privacyModeChanged(TaskFilterModel::onlyPrivate); this->setChecked(true); return;}
    if (a==publicAction) {emit privacyModeChanged(TaskFilterModel::onlyPublic); this->setChecked(true); return;}
}
