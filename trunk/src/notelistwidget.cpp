#include <QHeaderView>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>
#include "notelistwidget.h"
#include "notemodel.h"
#include "notefiltermodel.h"

NoteListWidget::NoteListWidget(QWidget *parent) :
    QTreeView(parent)
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    noteListModel=new NoteModel(this);
    filterModel=new NoteFilterModel(this);

    //connect(filterModel,SIGNAL(taskListChanged(QList<Task>)),this,SIGNAL(taskListChanged(QList<Task>)));

    filterModel->setSourceModel(noteListModel);
    this->setModel(filterModel);
    this->header()->setMovable(false);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setDragEnabled(true);
}

void NoteListWidget::setSourceModel(NoteModel *model)
{
    noteListModel=model;

    filterModel->setSourceModel(noteListModel);
    this->setModel(filterModel);
}

void NoteListWidget::appendNote(Note note)
{
    noteListModel->appendNote(note);
}

QStringList NoteListWidget::categories()
{
    return noteListModel->categories();
}

void NoteListWidget::removeNote(int row,QModelIndex parent)
{
    noteListModel->removeRows(row,1,filterModel->mapToSource(parent));
}

void NoteListWidget::currentChanged(QModelIndex current,QModelIndex last)
{
    emit selectedItemChanged(current);
    QTreeView::currentChanged(current,last);
}

Note NoteListWidget::getNote(QModelIndex index)
{
    index=filterModel->mapToSource(index);
    return noteListModel->getItem(index);
}

void NoteListWidget::replaceNote(QModelIndex index,Note note)
{
    index=filterModel->mapToSource(index);
    noteListModel->replaceNote(index,note);
}

void NoteListWidget::setNameFilter(QString filter)
{
   filterModel->setNameFilter(filter);
}

void NoteListWidget::startDrag()
{
    QModelIndex dragIndex=this->filterModel->mapToSource(indexAt(this->startDragPos));
    if (!dragIndex.isValid()) return;
    if (!dragIndex.parent().isValid()) return;
    Note note=noteListModel->getItem(dragIndex);

    QByteArray *data=new QByteArray;
    QDataStream buf(data,QIODevice::WriteOnly);
    buf<<note;

    QMimeData *dragData=new QMimeData;
    dragData->setData("application/ztl-note",*data);
    delete data;

    QDrag *drag=new QDrag(this);
    drag->setMimeData(dragData);
    QPixmap pix(":/icons/note");

    drag->setPixmap(pix);
    if (drag->exec(Qt::MoveAction)==Qt::MoveAction)
    {
        noteListModel->removeRows(dragIndex.row(),1,dragIndex.parent());
    }
}

void NoteListWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button()== Qt::LeftButton)
    {
        startDragPos=e->pos();
    }
    QTreeView::mousePressEvent(e);
}

void NoteListWidget::mouseMoveEvent(QMouseEvent *e)
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
    QTreeView::mouseMoveEvent(e);
}

void NoteListWidget::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasFormat("application/ztl-note"))
    {
        e->acceptProposedAction();
    }
}

void NoteListWidget::dragMoveEvent(QDragMoveEvent *e)
{
    e->acceptProposedAction();
}

void NoteListWidget::dropEvent(QDropEvent *e)
{
    QModelIndex dropIndex=filterModel->mapToSource(indexAt(e->pos()));
    if (!dropIndex.isValid()) return;
    QByteArray data(e->mimeData()->data("application/ztl-note"));
    QDataStream buf(&data,QIODevice::ReadOnly);
    Note note;
    buf>>note;
    QString currentCategory;
    if (dropIndex.parent().isValid())
    {
        currentCategory=dropIndex.parent().data(0).toString();
    } else currentCategory=dropIndex.data(0).toString();
    if (currentCategory!=note.category())
    {
        note.setCategory(currentCategory);
        this->appendNote(note);
        e->acceptProposedAction();
    }
}

void NoteListWidget::setPrivacyFilterState(NoteFilterModel::privacyFilterMode mode)
{
    filterModel->setPrivacyFilterMode(mode);
}

void NoteListWidget::togglePrivateNote(QModelIndex index)
{
    index=filterModel->mapToSource(index);

    Note note=noteListModel->getItem(index);
    noteListModel->removeRows(index.row(),0,index.parent());
    note.setPrivate(!note.isPrivate());
    noteListModel->appendNote(note);
}
