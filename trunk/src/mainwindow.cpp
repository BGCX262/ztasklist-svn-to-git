#include <QMessageBox>
#include <QFileDialog>
#include <QShortcut>
#include <QSplitter>
#include <QDebug>

#include "trackingnotemodel.h"
#include "noteeditdialog.h"
#include "mainwindow.h"
#include "taskcontextmenu.h"
#include "trayicon.h"
#include "trackingtaskmodel.h"
#include "notemodel.h"
#include "ui_mainwindow.h"

#include "programsettings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->taskListWidget,SIGNAL(selectedItemChanged(QModelIndex)),this,SLOT(selectedTaskChanged(QModelIndex)));
    connect(ui->noteListWidget,SIGNAL(selectedItemChanged(QModelIndex)),this,SLOT(selectedNoteChanged(QModelIndex)));
    connect(ui->taskListWidget,SIGNAL(taskListChanged(QList<Task>)),this,SLOT(taskListChanged(QList<Task>)));

    ui->noteBar->hide();
    ui->noteFilterWidget->hide();
    ui->taskListWidget->setSourceModel(TrackingTaskModel::getSharedModel());
    ui->taskListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->taskListWidget->setCompleteHidden(false);
    ui->taskListWidget->setFocus();
    this->taskListChanged(ui->taskListWidget->publicTasks()+ui->taskListWidget->privateTasks());

    connect(ui->taskListWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenuRequested(QPoint)));
    contextMenu=new TaskContextMenu(this);
    connect(contextMenu,SIGNAL(toggleComplete()),this,SLOT(toggleCurrentTask()));
    connect(contextMenu,SIGNAL(togglePrivate()),this,SLOT(togglePrivateCurrentTask()));
    connect(contextMenu,SIGNAL(priorityChanged(int)),this,SLOT(setCurrentPriority(int)));
    connect(contextMenu,SIGNAL(repeatChaged(Task::RepeatType)),this,SLOT(setCurrentRepeat(Task::RepeatType)));
    connect(contextMenu,SIGNAL(removeTask()),this,SLOT(on_actionRemove_task_triggered()));

    icon=new TrayIcon(this);
    icon->show();
    connect(icon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(activate(QSystemTrayIcon::ActivationReason)));

    QHBoxLayout *tabLayout=new QHBoxLayout(ui->tabWidget);
    tabLayout->setMargin(0);
    tabLayout->setSpacing(0);

    tabLayout->addSpacing(150);
    tabLayout->addWidget(new QWidget(ui->tabWidget),0,Qt::AlignLeft|Qt::AlignTop);
    tabLayout->addWidget(ui->taskFilterWidget,1,Qt::AlignTop);
    tabLayout->addWidget(ui->noteFilterWidget,1,Qt::AlignTop);

    connect(ui->completeBox,SIGNAL(toggled(bool)),ui->taskListWidget,SLOT(setCompleteHidden(bool)));
    connect(ui->taskFilterEdit,SIGNAL(textChanged(QString)),ui->taskListWidget,SLOT(setNameFilter(QString)));

    connect(ui->actionToggle_completion,SIGNAL(triggered()),this,SLOT(toggleCurrentTask()));
    connect(ui->actionToggleTask_privacy,SIGNAL(triggered()),this,SLOT(togglePrivateCurrentTask()));
    connect(ui->actionToggleNote_privacy,SIGNAL(triggered()),this,SLOT(togglePrivateCurrentNote()));

    QShortcut *tasksShortcut=new QShortcut(QKeySequence("Alt+1"),this);
    connect(tasksShortcut,SIGNAL(activated()),this,SLOT(showTasks()));

    QShortcut *notesShortcut=new QShortcut(QKeySequence("Alt+2"),this);
    connect(notesShortcut,SIGNAL(activated()),this,SLOT(showNotes()));

    ui->noteListWidget->setSourceModel(TrackingNoteModel::getSharedModel());

    connect(ui->noteFilterEdit,SIGNAL(textChanged(QString)),ui->noteListWidget,SLOT(setNameFilter(QString)));

    QSplitter *taskSplitter=new QSplitter(Qt::Horizontal,this);
    taskSplitter->addWidget(ui->taskListWidget);
    taskSplitter->addWidget(ui->editTaskBox);
    ui->gridLayout_2->addWidget(taskSplitter);

    QSplitter *noteSplitter=new QSplitter(Qt::Horizontal,this);
    noteSplitter->addWidget(ui->noteListWidget);
    noteSplitter->addWidget(ui->groupBox);
    ui->gridLayout_5->addWidget(noteSplitter);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::activate(QSystemTrayIcon::ActivationReason reason)
{
    if (reason==QSystemTrayIcon::Trigger)
    {
        this->setVisible(!this->isVisible());
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index==0)
    {
        ui->taskBar->show();
        ui->noteBar->hide();
        ui->taskFilterWidget->show();
        ui->noteFilterWidget->hide();
    }

    if (index==1)
    {
        ui->taskBar->hide();
        ui->noteBar->show();
        ui->taskFilterWidget->hide();
        ui->noteFilterWidget->show();
    }

}

void MainWindow::on_actionNew_task_triggered()
{
    TaskDialog newTaskDialog;
    if (newTaskDialog.exec())
    {
        Task task=newTaskDialog.task();
        if (task.description().isEmpty()) return;
        task.setCreateDate(QDate::currentDate());
        ui->taskListWidget->appendTask(task);
    }
}

void MainWindow::on_actionRemove_task_triggered()
{
    bool res=true;
    QModelIndex index=ui->taskListWidget->currentIndex();
    if (ProgramSettings::settings()->removeTaskQuestion())
        res=QMessageBox::warning(0,tr("Really remove?"),
                                 tr("Do you really want to remove task <b>\"%1\"</b>?").arg(index.data(Task::NameRole).toString()),
            QMessageBox::Yes| QMessageBox::Default,
            QMessageBox::No)==QMessageBox::Yes;

   if(res)
   {
       ui->taskListWidget->removeTask(ui->taskListWidget->currentIndex());
   }

}

void MainWindow::on_taskFilterBox_currentIndexChanged(int index)
{
    switch (index)
    {
        case 0: ui->taskListWidget->setPrivacyFilterState(TaskFilterModel::all);  break;//All tasks
        case 1:ui->taskListWidget->setPrivacyFilterState(TaskFilterModel::onlyPublic);   break;//Only public
        case 2:ui->taskListWidget->setPrivacyFilterState(TaskFilterModel::onlyPrivate);  break;//Only private
    }
}

void MainWindow::selectedTaskChanged(QModelIndex index)
{
    if (!index.isValid()) return;

    ui->saveTaskButton->setEnabled(true);
    ui->cancelTaskButton->setEnabled(true);
    ui->taskEditWidget->setEnabled(true);

    ui->actionRemove_task->setEnabled(true);
    ui->actionToggle_completion->setEnabled(true);
    ui->actionToggleTask_privacy->setEnabled(true);

    bool completed_=index.data(Task::CompleteStatusRole).toBool();
    bool private_=index.data(Task::PrivateRole).toBool();

    ui->actionToggle_completion->setChecked(completed_);
    ui->actionToggleTask_privacy->setChecked(private_);

    ui->taskEditWidget->setTask(ui->taskListWidget->task(ui->taskListWidget->currentIndex().row()));
}

void MainWindow::on_actionRemove_completed_triggered()
{
    bool res=QMessageBox::warning(0,tr("Really delete?"),
        tr("Do you really want to delete the completed tasks?"),
        QMessageBox::Yes| QMessageBox::Default,
        QMessageBox::No)==QMessageBox::Yes;

    if (res) ui->taskListWidget->removeCompleted();
}

void  MainWindow::setCurrentPriority(int p)
{
    QModelIndex index=ui->taskListWidget->currentIndex();
    ui->taskListWidget->setPriority(index,p);
}

void MainWindow::togglePrivateCurrentTask()
{
    if (!ui->taskListWidget->currentIndex().isValid()) return;
    ui->taskListWidget->togglePrivateTask(ui->taskListWidget->currentIndex());
    selectedTaskChanged(ui->taskListWidget->currentIndex());
}

void MainWindow::togglePrivateCurrentNote()
{
    ui->noteListWidget->togglePrivateNote(ui->noteListWidget->currentIndex());
    selectedNoteChanged(ui->taskListWidget->currentIndex());
}

void MainWindow::toggleCurrentTask()
{
    ui->taskListWidget->toggleTask(ui->taskListWidget->currentIndex());
    selectedTaskChanged(ui->taskListWidget->currentIndex());
}

void MainWindow::customContextMenuRequested(const QPoint &)
{
    if (ui->taskListWidget->indexAt(ui->taskListWidget->mapFromGlobal(QCursor::pos())).isValid())
    {
        if (contextMenu)
            contextMenu->exec(QCursor::pos());
    }
}

void MainWindow::setCurrentRepeat(Task::RepeatType v)
{
    QModelIndex index=ui->taskListWidget->currentIndex();
    ui->taskListWidget->setRepeat(index,v);
}

void MainWindow::on_saveTaskButton_clicked()
{
    ui->taskListWidget->replaceTask(ui->taskListWidget->currentIndex(),ui->taskEditWidget->task());
    this->selectedTaskChanged(ui->taskListWidget->currentIndex());
}

void MainWindow::on_cancelTaskButton_clicked()
{
    ui->taskEditWidget->setTask(ui->taskListWidget->task(ui->taskListWidget->currentIndex().row()));
}

void MainWindow::showTasks()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::showNotes()
{
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::taskListChanged(QList<Task> list)
{
    int count=list.count();
    if (count==0)
    {
        ui->taskEditWidget->clear();
        ui->taskEditWidget->setDisabled(true);
        ui->saveTaskButton->setDisabled(true);
        ui->cancelTaskButton->setDisabled(true);
        ui->actionRemove_task->setDisabled(true);
        ui->actionToggle_completion->setDisabled(true);
        ui->actionToggleTask_privacy->setDisabled(true);
        ui->actionRemove_completed->setDisabled(true);
    }
    else
    {
        /*ui->saveTaskButton->setEnabled(true);
        ui->cancelTaskButton->setEnabled(true);
        ui->taskEditWidget->setEnabled(true);
        ui->actionRemove_task->setEnabled(true);
        ui->actionToggle_completion->setEnabled(true);*/
        ui->actionRemove_completed->setEnabled(true);
    }
}

void MainWindow::on_actionNew_note_triggered()
{
    NoteEditDialog dialog(this);
    if (dialog.exec())
    {
        Note note=dialog.note();
        note.setCreated(QDateTime::currentDateTime());
        ui->noteListWidget->appendNote(note);
    }
}

void MainWindow::on_actionRemove_note_triggered()
{
    bool res=true;
    QModelIndex index=ui->noteListWidget->currentIndex();
    if (ProgramSettings::settings()->removeTaskQuestion())
        res=QMessageBox::warning(0,tr("Really remove?"),
                                 tr("Do you really want to remove note <b>\"%1\"</b>?").arg(index.data().toString()),
            QMessageBox::Yes| QMessageBox::Default,
            QMessageBox::No)==QMessageBox::Yes;

   if(res)
   {
       ui->noteListWidget->removeNote(ui->noteListWidget->currentIndex().row(),ui->noteListWidget->currentIndex().parent());
   }
}

void MainWindow::selectedNoteChanged(QModelIndex index)
{
    if (index.parent().isValid())
    {
        ui->groupBox->setDisabled(false);
        ui->noteEditWidget->setNote(ui->noteListWidget->getNote(index));
        ui->actionToggleNote_privacy->setEnabled(true);
        ui->actionToggleNote_privacy->setChecked(ui->noteListWidget->getNote(index).isPrivate());

    } else
    {
        ui->groupBox->setDisabled(true);
        ui->actionToggleNote_privacy->setEnabled(false);
        ui->noteEditWidget->clear();
        ui->actionToggleNote_privacy->setChecked(false);

    }
}

void MainWindow::on_cancelNoteButton_clicked()
{
    selectedNoteChanged(ui->noteListWidget->currentIndex());
}

void MainWindow::on_saveNoteButton_clicked()
{
    ui->noteListWidget->replaceNote(ui->noteListWidget->currentIndex(),ui->noteEditWidget->note());
}

void MainWindow::on_noteFilterBox_currentIndexChanged(int index)
{
    switch (index)
    {
        case 0: ui->noteListWidget->setPrivacyFilterState(NoteFilterModel::all);  break;//All tasks
        case 1: ui->noteListWidget->setPrivacyFilterState(NoteFilterModel::onlyPublic);   break;//Only public
        case 2: ui->noteListWidget->setPrivacyFilterState(NoteFilterModel::onlyPrivate);  break;//Only private
    }
}
