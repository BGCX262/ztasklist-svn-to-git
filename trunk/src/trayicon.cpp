#include <QApplication>
#include <QToolTip>
#include <QMessageBox>
#include <QWidgetAction>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QComboBox>
#include <QDomDocument>
#include <QDebug>

#include "trayicon.h"
#include "settingsdialog.h"
#include "task.h"
#include "iconwrapper.h"
#include "taskcontextmenu.h"
#include "transparentbutton.h"
#include "menubuttonaction.h"
#include "aboutdialog.h"
#include "filterbutton.h"
#include "noteeditdialog.h"
#include "task.h"
#include "trackingtaskmodel.h"
#include "trackingnotemodel.h"
#include "datamanager.h"
#include "another/qxtglobalshortcut.h"

#include "programsettings.h"

TrayIcon::TrayIcon(QObject *parent):QSystemTrayIcon(parent)
{
    this->setIcon(QIcon(":/icons/icon"));

    this->setContextMenu(makeTaskMenu());
    makeNoteMenu();
    connect(this,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(activate(QSystemTrayIcon::ActivationReason)));
    QxtGlobalShortcut *newTaskShortCut=new QxtGlobalShortcut(QKeySequence("Alt+F10"),this);
    connect (newTaskShortCut,SIGNAL(activated()),this,SLOT(newTask()));

    QxtGlobalShortcut *newNoteShortCut=new QxtGlobalShortcut(QKeySequence("Alt+F11"),this);
    connect (newNoteShortCut,SIGNAL(activated()),this,SLOT(newNote()));
}

QMenu *TrayIcon::makeTaskMenu()
{
    taskMenu=new QMenu("TrayIcon");

    buttonBar=new MenuButtonAction(taskMenu);
    QToolButton *newTaskButton=new QToolButton(taskMenu);
    newTaskButton->setIcon(IconWrapper::addIcon());
    newTaskButton->setText(tr("New task"));
    newTaskButton->setToolTip(tr("Add new task"));
    connect(newTaskButton,SIGNAL(clicked()),this,SLOT(newTask()));

    QToolButton *settingsButton=new QToolButton(taskMenu);
    settingsButton->setIcon(IconWrapper::prefIcon());
    settingsButton->setText(tr("Settings"));
    settingsButton->setToolTip(tr("Program settings"));
    connect(settingsButton,SIGNAL(clicked()),this,SLOT(openSettings()));

    QToolButton *exitButton=new QToolButton(taskMenu);
    exitButton->setIcon(IconWrapper::exitIcon());
    exitButton->setText(tr("Exit"));
    exitButton->setToolTip(tr("Exit"));
    connect(exitButton,SIGNAL(clicked()),this,SLOT(exit()));

    QToolButton *aboutButton=new QToolButton(taskMenu);
    aboutButton->setIcon(IconWrapper::aboutIcon());
    aboutButton->setText(tr("About"));
    aboutButton->setToolTip(tr("About program"));
    connect(aboutButton,SIGNAL(clicked()),this,SLOT(about()));

    menuListWidget=new QWidgetAction(taskMenu);

    taskListWidget=new TaskListWidget(taskMenu);
    taskListWidget->setSourceModel(TrackingTaskModel::getSharedModel());
    taskListWidget->setMinimumHeight(300);
    taskListWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
    taskListWidget->setCompleteHidden(true);
    taskListWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    contextMenu=new TaskContextMenu(taskListWidget);
    connect(contextMenu,SIGNAL(toggleComplete()),this,SLOT(toggleCurrentTask()));
    connect(contextMenu,SIGNAL(togglePrivate()),this,SLOT(togglePrivateCurrentTask()));
    connect(contextMenu,SIGNAL(priorityChanged(int)),this,SLOT(setCurrentPriority(int)));
    connect(contextMenu,SIGNAL(removeTask()),this,SLOT(removeCurrentTask()));
    connect(contextMenu,SIGNAL(repeatChaged(Task::RepeatType)),this,SLOT(setCurrentRepeat(Task::RepeatType)));

    menuListWidget->setDefaultWidget(taskListWidget);
    connect(taskListWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenuRequested(QPoint)));
    connect(taskListWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(itemDoubleClicked(QModelIndex)));

    QVBoxLayout *layout=new QVBoxLayout(taskListWidget);

    newTaskTButton=new TransparentButton(IconWrapper::addIcon(),taskListWidget,64);
    newTaskTButton->setToolTip(tr("Add new task"));

    connect(newTaskTButton,SIGNAL(clicked()),this,SLOT(newTask()));

    layout->addWidget(new QWidget(taskMenu),1,Qt::AlignCenter|Qt::AlignVCenter);
    layout->addWidget(newTaskTButton,1,Qt::AlignCenter|Qt::AlignVCenter);

    connect(taskListWidget,SIGNAL(mouseEntered()),this,SLOT(mouseEnterInList()));
    connect(taskListWidget,SIGNAL(mouseLeave()),this,SLOT(mouseLeaveFromList()));

    FilterButton *filterButton= new FilterButton(taskMenu);
    filterButton->setPrivateEnable(ProgramSettings::settings()->privateEnable());
    connect(filterButton,SIGNAL(onlyTodayModeChanged(bool)),taskListWidget,SLOT(setOnlyTodayShow(bool)));
    connect(filterButton,SIGNAL(privacyModeChanged(TaskFilterModel::privacyFilterMode)),
            taskListWidget,SLOT(setPrivacyFilterState(TaskFilterModel::privacyFilterMode)));

    connect(ProgramSettings::settings(),SIGNAL(privateTasksToggled(bool)),filterButton,SLOT(setPrivateEnable(bool)));

    buttonBar->addWidget(newTaskButton);
    buttonBar->addWidget(settingsButton);
    buttonBar->addWidget(filterButton);
    buttonBar->addSpace();
    buttonBar->addWidget(aboutButton);
    buttonBar->addWidget(exitButton);

    if (ProgramSettings::settings()->buttonBarOnTop()) taskMenu->addAction(buttonBar);
    taskMenu->addAction(menuListWidget);
    if (!ProgramSettings::settings()->buttonBarOnTop()) taskMenu->addAction(buttonBar);

    connect(ProgramSettings::settings(),SIGNAL(buttonBatOnTopStateChanged(bool)),this,SLOT(setButtonBarOnTop(bool)));
    return taskMenu;
}

QMenu *TrayIcon::makeNoteMenu()
{
    noteMenu=new QMenu();
    updateNotes(DataManager::dataManager()->notes());
    connect(DataManager::dataManager(),SIGNAL(noteListChanged(QHash<QString,QList<Note> >)),this,SLOT(updateNotes(QHash<QString,QList<Note> >)));
    connect(TrackingNoteModel::getSharedModel(),SIGNAL(noteListChanged(QHash<QString,QList<Note> >)),this,SLOT(updateNotes(QHash<QString,QList<Note> >)));

    return noteMenu;
}

TrayIcon::~TrayIcon()
{
    delete taskMenu;
    delete noteMenu;
}

void TrayIcon::updateNotes(QHash<QString,QList<Note> > notesHash)
{
    noteMenu->clear();

    QAction *newNote=new QAction(IconWrapper::addIcon(),tr("New note"),noteMenu);
    connect(newNote,SIGNAL(triggered()),this,SLOT(newNote()));
    noteMenu->addAction(newNote);

    noteMenu->addSeparator()->setText(tr("Notes"));

    QList<Note> notes;
    foreach (QList<Note> val,notesHash.values())
        notes.append(val);

    qSort(notes);
    notes=notes.mid(0,10);

    foreach(Note note,notes)
    {
        QAction *newAction=new QAction(QIcon(":/icons/note"),note.name(),noteMenu);
        QVariant data;
        data.setValue(note);
        newAction->setData(data);
        connect(newAction,SIGNAL(triggered()),this,SLOT(noteActionActivated()));
        noteMenu->addAction(newAction);
    }
}

void TrayIcon::TrayIcon::noteActionActivated()
{
    QAction *action=qobject_cast<QAction*>(sender());
    Note note=action->data().value<Note>();
    NoteEditDialog dialog;
    dialog.setNote(note);
    if (dialog.exec())
    {
        QModelIndex index=TrackingNoteModel::getSharedModel()->indexOf(note);
        TrackingNoteModel::getSharedModel()->replaceNote(index,dialog.note());
    }

}

void TrayIcon::newTask()
{
    TaskDialog dialog;
    if (dialog.exec())
    {
        Task task=dialog.task();
        task.setCreateDate(QDate::currentDate());
        if (task.description().isEmpty()) return;
        taskListWidget->appendTask(task);
    }
}

void TrayIcon::mouseEnterInList()
{
    if (taskListWidget->uncompleteCount()==0)
        newTaskTButton->show();
}

void TrayIcon::mouseLeaveFromList()
{
    if (taskListWidget->uncompleteCount()==0)
        newTaskTButton->hide();
}

void TrayIcon::openSettings()
{
    SettingsDialog settingsDialog;
    settingsDialog.exec();
}

void TrayIcon::activate(QSystemTrayIcon::ActivationReason reason)
{
    if (reason==QSystemTrayIcon::MiddleClick)
    {
        noteMenu->exec(QCursor::pos());
    }
}

void TrayIcon::itemDoubleClicked(QModelIndex index)
{
    TaskDialog dialog;
    Task editTask=taskListWidget->task(index.row());
    dialog.setTask(editTask);
    if (dialog.exec())
    {
        Task newTask=dialog.task();
        if (editTask!=newTask)
            taskListWidget->replaceTask(index,newTask);
    }
}

void TrayIcon::customContextMenuRequested(const QPoint &)
{
    if (taskListWidget->indexAt(taskListWidget->mapFromGlobal(QCursor::pos())).isValid())
    {
        if (contextMenu)
            contextMenu->exec(QCursor::pos());
    }
}

void TrayIcon::removeCurrentTask()
{
    bool res=true;
    QModelIndex index=taskListWidget->currentIndex();
    if (ProgramSettings::settings()->removeTaskQuestion())
        res=QMessageBox::warning(0,tr("Really remove?"),
                                 tr("Do you really want to remove task <b>\"%1\"</b>?").arg(index.data(Task::NameRole).toString()),
            QMessageBox::Yes| QMessageBox::Default,
            QMessageBox::No)==QMessageBox::Yes;

   if(res) taskListWidget->removeTask(taskListWidget->currentIndex());
}

void  TrayIcon::setCurrentPriority(int p)
{
    QModelIndex index=taskListWidget->currentIndex();
    taskListWidget->setPriority(index,p);
}

void TrayIcon::togglePrivateCurrentTask()
{
    QModelIndex index=taskListWidget->currentIndex();
    taskListWidget->togglePrivateTask(index);
}

void TrayIcon::toggleCurrentTask()
{
    QModelIndex index=taskListWidget->currentIndex();
    taskListWidget->toggleTask(index);
}

void TrayIcon::about()
{
    AboutDialog about;
    about.exec();
}

void TrayIcon::setButtonBarOnTop(bool v)
{
    taskMenu->removeAction(buttonBar);
    if (v)
    {
        taskMenu->insertAction(menuListWidget,buttonBar);
    } else
    {
        taskMenu->addAction(buttonBar);
    }
}

void TrayIcon::setCurrentRepeat(Task::RepeatType v)
{
    QModelIndex index=taskListWidget->currentIndex();
    taskListWidget->setRepeat(index,v);
}

bool TrayIcon::event(QEvent *event)
{
    if (event->type()==QEvent::ToolTip)
    {
            QList<Task> tasks=taskListWidget->publicTasks();
            if (tasks.isEmpty()) return false;
            QString tasksDeskription="<h2>Your tasks</h2>";
            foreach (Task task,tasks)
            {
                if (!task.completed())
                {
                    tasksDeskription+="<b>"+task.name()+"</b> ";
                    tasksDeskription+=task.description();
                    tasksDeskription+="<br>";
                }
            }
            QToolTip::showText(QCursor::pos(),tasksDeskription);
            return true;
    } else
        return QObject::event(event);
}

void TrayIcon::exit()
{
    bool res=true;
    if (ProgramSettings::settings()->exitQuestion())
    {
        res=QMessageBox::question(0,tr("Really exit?"),
                          tr("Do your really want to exit?"),
                          QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes;
    }
    if (res) qApp->exit(0);
}

void TrayIcon::newNote()
{
    NoteEditDialog dialog;
    if (dialog.exec())
    {
        TrackingNoteModel::getSharedModel()->appendNote(dialog.note());
    }
}
