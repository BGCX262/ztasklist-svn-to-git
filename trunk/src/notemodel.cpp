#include <QIcon>

#include "notemodel.h"


NoteModel::NoteModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

NoteModel::~NoteModel()
{

}

QVariant NoteModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role!=Qt::DecorationRole)
        return QVariant();

    if (role==Qt::DecorationRole && index.column()!=0) return QVariant();

    if (index.internalId()==0)
    {
        if (role==Qt::DecorationRole && index.column()==0)
        {
            return QIcon(":/icons/book");
        } else
        if (index.column()==0) return dataHash.keys()[index.row()];
        else return QVariant();
    }else
    {
        Note note=getItem(index);
        if (role==Qt::DecorationRole && index.column()==0)
        {
            if (note.isPrivate())
                return QIcon(":/icons/private");
            else return QIcon(":/icons/note");
        } else
        switch (index.column())
        {
            case 0:return note.name();
             case 1:return note.modified();
              case  2:return note.created();
        }
    }
    return QVariant();
}

QVariant NoteModel::headerData(int section, Qt::Orientation orientation,
             int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:return tr("Name");
            case 1:return tr("Modified");
            case 2:return tr("Created");
        }
    }

    return QVariant();
}

QModelIndex NoteModel::index(int row, int column,
           const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    if (!parent.isValid())
    {
        return createIndex(row, column,(quint32)0);
    }
    else
    {
        QString cat=getItem(parent).name();
        return createIndex(row,column,(quint32)1+dataHash.keys().indexOf(cat));
    }

}

QModelIndex NoteModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    if (index.internalId()==0) return QModelIndex();
    else
    {
        QString cat=getItem(index).name();
        int row=index.internalId()-1;
        return createIndex(row,index.column(),(quint32)0);
    }

}

int NoteModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return dataHash.keys().count();
    }
    if (parent.internalId()==0)
    {
        return dataHash[dataHash.keys()[parent.row()]].count();
    }
    else
    {
        return 0;
    }

}

int NoteModel::columnCount(const QModelIndex &) const
{
    return 3;
}

Qt::ItemFlags NoteModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void NoteModel::appendNote(Note note)
{

    if (dataHash.contains(note.category()))
        dataHash[note.category()].append(note);
    else
    {
        QList<Note> list;
        list.append(note);
        dataHash.insert(note.category(),list);
    }

    emit dataChanged(index(0,0),index(dataHash.keys().count()-1,2));
    emit noteListChanged(dataHash);
}

bool NoteModel::removeRows(int position, int, const QModelIndex &parent)//Removes only one row!
{

    if (!parent.isValid())
    {
        beginRemoveRows(parent, position, position + 1);
        dataHash.remove(dataHash.keys()[position]);
        endRemoveRows();
        emit dataChanged(index(0,0),index(dataHash.keys().count()-1,2));
    }
    else
    {
        QString cat=dataHash[getItem(parent).name()].at(position).category();
        beginRemoveRows(parent, position, position + 1);
        dataHash[getItem(parent).name()].removeAt(position);
        endRemoveRows();
        emit dataChanged(index(0,0,parent),index(rowCount(parent)-1,2,parent));
        if (dataHash[cat].count()==0)
        {
            removeRows(dataHash.keys().indexOf(cat),1);
        }

    }



    emit noteListChanged(dataHash);
    return true;
}

Note NoteModel::getItem(const QModelIndex &index) const
{
    int n=index.internalId();

    if (n==0)
    {
        Note catNote;
        catNote.setName(dataHash.keys()[index.row()]);
        return catNote;
    } else
    {
        QString cat=dataHash.keys()[index.internalId()-1];
        Note res=dataHash[cat][index.row()];
        return res;
    }
}

void NoteModel::clear()
{
    beginRemoveRows(QModelIndex(),0,dataHash.keys().count()-1);
    dataHash.clear();
    endRemoveRows();
}

void NoteModel::setNotes(QHash<QString, QList<Note> > notes)
{
    this->clear();
    beginInsertRows(QModelIndex(),0,notes.keys().count());
        dataHash=notes;
    endInsertRows();
    emit dataChanged(index(0,0),index(dataHash.keys().count(),2));
}

QStringList NoteModel::categories()
{
    return dataHash.keys();
}

void NoteModel::replaceNote(QModelIndex index,Note note)
{
    this->removeRows(index.row(),1,index.parent());
    this->appendNote(note);
}

QModelIndex NoteModel::indexOf(Note note) const
{
    QList<Note> notes;
    foreach (QList<Note> val,dataHash.values())
        notes.append(val);

    if (!notes.contains(note)) return QModelIndex();
    else
    {
        int catID=dataHash.keys().indexOf(note.category());
        QModelIndex parentIndex=createIndex(catID,0,0);
        return index(dataHash[note.category()].indexOf(note),0,parentIndex);
    }

}
