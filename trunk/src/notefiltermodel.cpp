#include <QDebug>
#include "notefiltermodel.h"

NoteFilterModel::NoteFilterModel(QObject *parent):QSortFilterProxyModel(parent)
{
    privacyFilterMode_=NoteFilterModel::all;
}


void NoteFilterModel::setSourceModel(NoteModel *sourceModel)
{
    connect(sourceModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(invalidate()));
    connect(sourceModel,SIGNAL(noteListChanged(QHash<QString,QList<Note> >)),this,SIGNAL(noteListChanged(QHash<QString,QList<Note> >)));
    sourceModel_=sourceModel;
    QSortFilterProxyModel::setSourceModel(sourceModel);
}

bool NoteFilterModel::filterAcceptsRow(int row, const QModelIndex &parent) const
{
    if (!parent.isValid()) return true;
    if (!parent.child(row,0).isValid()) return true;

    Note note= sourceModel_->getItem(parent.child(row,0));
    if (privacyFilterMode_!=all)
    switch (privacyFilterMode_)
    {
    case all: break;
    case onlyPublic:
        if (note.isPrivate()) return false; break;
    case onlyPrivate:
        if (!note.isPrivate()) return false;break;
    }

    if (parent.child(row,0).data(0).toString().toLower().contains(nameFilter_))
        return true;
    else return false;
}


void NoteFilterModel::setNameFilter(QString filter)
{
    nameFilter_=filter.toLower();
    invalidate();
}

void NoteFilterModel::setPrivacyFilterMode(privacyFilterMode mode)
{
    privacyFilterMode_=mode;
    this->invalidate();
}
