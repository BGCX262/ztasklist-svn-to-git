#include <QToolTip>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>

#include "tasklistwidget.h"
#include "taskdelegate.h"
#include "taskmodel.h"

TaskListWidget::TaskListWidget(QWidget *parent) :
    QListView(parent)
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    taskListModel=new TaskModel(this);
    filterModel=new TaskFilterModel(this);

    connect(filterModel,SIGNAL(taskListChanged(QList<Task>)),this,SIGNAL(taskListChanged(QList<Task>)));

    this->setItemDelegate(new TaskDelegate(this));
    this->setMinimumWidth(340);

    filterModel->setSourceModel(taskListModel);
      this->setModel(filterModel);

}

void TaskListWidget::setSourceModel(TaskModel *model)
{
    taskListModel=model;

    filterModel->setSourceModel(taskListModel);
    this->setModel(filterModel);
}

TaskListWidget::~TaskListWidget()
{

}

int TaskListWidget::count()
{
    return model()->rowCount();
}

int TaskListWidget::uncompleteCount()
{
    return model()->uncompleteCount();
}

void TaskListWidget::appendTask(Task task)
{
    model()->appendTask(task);
}

void TaskListWidget::replaceTask(QModelIndex index,Task replace)
{
    model()->replaceTask(index,replace);

    this->selectionModel()->clearSelection();
    this->selectionModel()->setCurrentIndex(model()->indexFromTask(replace),QItemSelectionModel::Select);
}

void TaskListWidget::removeTask(QModelIndex index)
{
    if (!index.isValid()) return;
    model()->removeRow(index.row());
}

void TaskListWidget::toggleTask(QModelIndex index)
{
    Task task=model()->taskAt(index.row());
    task.setCompleted(!task.completed());
    this->replaceTask(index,task);
}

void TaskListWidget::toggleTask(int row)
{
    QModelIndex index=model()->index(row);
    this->toggleTask(index);
}

void TaskListWidget::toggleTask(Task task)
{
    QModelIndex index=model()->indexFromTask(task);
    task.setCompleted(!task.completed());
    this->replaceTask(index,task);
}

void TaskListWidget::togglePrivateTask(QModelIndex index)
{
    if (!index.isValid()) return;
    Task task=model()->taskAt(index.row());
    task.setPrivate(!task.isPrivate());
    this->replaceTask(index,task);
}

QList<Task> TaskListWidget::publicTasks()
{
    return model()->getPublicTasks();
}

QList<Task> TaskListWidget::privateTasks()
{
    return model()->getPrivateTasks();
}

void TaskListWidget::setTasks(QList<Task> tasks,bool clear)
{
    if (clear)this->clear();
    model()->setTasks(tasks);
    selectionModel()->setCurrentIndex(model()->index(0),QItemSelectionModel::Select);
}

Task TaskListWidget::task(int row)
{
    return model()->taskAt(row);
}

void TaskListWidget::setCompleteHidden(bool state)
{
    filterModel->setCompletedHide(state);
}

void TaskListWidget::clear()
{
    model()->clear();
}

void TaskListWidget::removeCompleted()
{
    model()->removeCompleted();
}

void TaskListWidget::removeDuplicates()
{
    model()->removeDuplicates();
}

void TaskListWidget::setPrivacyFilterState(TaskFilterModel::privacyFilterMode v)
{
    filterModel->setPrivacyFilterMode(v);
}

void TaskListWidget::setPriority(QModelIndex index,int p)
{
    Task task=model()->taskAt(index.row());
    task.setPriority(p);
    this->replaceTask(index,task);
}


void TaskListWidget::setOnlyTodayShow(bool v)
{
    filterModel->setNonTodayHide(v);
}

void TaskListWidget::setNameFilter(QString v)
{
    filterModel->setNameFilter(v);
}

void TaskListWidget::setRepeat(QModelIndex index,Task::RepeatType v)
{
    model()->setData(index,v,Task::RepeatRole);
}

void TaskListWidget::currentChanged(QModelIndex current,QModelIndex)
{
    emit selectedItemChanged(current);
}



TaskModel *TaskListWidget::model() const
{
    return taskListModel;
}

QString TaskListWidget::timeFromNow(QDateTime deadlineTime)
{
    QString res;
    int daysTo=abs(deadlineTime.daysTo(QDateTime::currentDateTime()));
    int secsTo=abs(deadlineTime.secsTo(QDateTime::currentDateTime()))%86400;
    int minutesTo=secsTo/60;
    int hoursTo=minutesTo/60;
    res=QString("%2 days %3 hours %4 minutes");
    minutesTo=minutesTo%60;
    res=res.arg(daysTo).arg(hoursTo).arg(minutesTo);
    return res;
}

bool TaskListWidget::event(QEvent *e)
{
    if (e->type()==QEvent::Enter)
    {
        emit mouseEntered();
    }

    if (e->type()==QEvent::Leave)
    {
        emit mouseLeave();
    }
    return QListView::event(e);
}

void TaskListWidget::startDrag()
{
    QModelIndex dragIndex=this->indexAt(this->startDragPos);
    Task task=model()->taskAt(dragIndex.row());

    QByteArray *data=new QByteArray;
    QDataStream buf(data,QIODevice::WriteOnly);
    buf<<task;

    QMimeData *dragData=new QMimeData;
    dragData->setData("application/ztl-task",*data);
    delete data;

    QDrag *drag=new QDrag(this);
    drag->setMimeData(dragData);
    QPixmap pix(task.priorityIcon());

    drag->setPixmap(pix);
    if (drag->exec(Qt::MoveAction)==Qt::MoveAction)
    {
        this->model()->removeRow(dragIndex.row());
    }
}

void TaskListWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button()== Qt::LeftButton)
    {
        startDragPos=e->pos();
    }
    QListView::mousePressEvent(e);
}

void TaskListWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        int distance=(e->pos()-startDragPos).manhattanLength();
        if (distance>QApplication::startDragDistance())
        {
            if (this->dragDropMode()!= QAbstractItemView::NoDragDrop)
            {
                startDrag();
            }
        }
    }
    QListView::mouseMoveEvent(e);
}

void TaskListWidget::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasFormat("application/ztl-task") && e->source()!=this)
    {
        e->acceptProposedAction();
    }
}

void TaskListWidget::dragMoveEvent(QDragMoveEvent *e)
{
    e->acceptProposedAction();
}

void TaskListWidget::dropEvent(QDropEvent *e)
{
    QByteArray data(e->mimeData()->data("application/ztl-task"));
    QDataStream buf(&data,QIODevice::ReadOnly);
    Task task;
    buf>>task;
    if (!this->publicTasks().contains(task))
    {
        this->appendTask(task);
        e->acceptProposedAction();
    }
}

bool TaskListWidget::viewportEvent(QEvent *event)
{
    if (event->type()==QEvent::ToolTip && this->indexAt(mapFromGlobal(QCursor::pos())).isValid())
    {
        QModelIndex currentIndex=this->indexAt(mapFromGlobal(QCursor::pos()));

        QString name=currentIndex.data(Task::NameRole).toString();
        QString description=currentIndex.data(Task::DescriptionRole).toString().replace("\n","<br>");
        QDateTime deadlineTime=currentIndex.data(Task::DeadlineRole).toDateTime();
        QString leftString;
        if(deadlineTime.isValid() && QDateTime::currentDateTime()<deadlineTime)
        {
            leftString="<br><b>Left:</b>"+this->timeFromNow(deadlineTime);
        }

        QString toolTipString=
            QString("<h3><center>%1</center></h3><i><p style='white-space:nowpar'>%2</p></i>%3").
            arg(name).arg(description).arg(leftString);

        QToolTip::showText(QCursor::pos(),toolTipString,this);
        return true;
    }  else return QListView::viewportEvent(event);

}

void TaskListWidget::paintEvent(QPaintEvent *e)
{
    if ((model()->uncompleteCount()==0 && filterModel->completedHide()) || model()->rowCount()==0)
    {
        QPainter painter(this->viewport());
        painter.setOpacity(.5);

        painter.setFont(QFont("",25));

        painter.drawText(this->rect(),Qt::AlignCenter,tr("List is empty..."));
    } else QListView::paintEvent(e);
}
