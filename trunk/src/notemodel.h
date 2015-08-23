#ifndef NOTEMODEL_H
#define NOTEMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

#include "note.h"

//class NoteItem;

class NoteModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit NoteModel(QObject *parent = 0);
    ~NoteModel();

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                 int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex indexOf(Note) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool removeRows(int position, int rows, const QModelIndex &parent=QModelIndex());
    Note getItem(const QModelIndex &index) const;
    void clear();
    void replaceNote(QModelIndex index,Note);

    void appendNote(Note note);
    QStringList categories();


private:
    QHash<QString,QList<Note> > dataHash;

public slots:
    void setNotes(QHash<QString, QList<Note> >);

signals:
    void noteListChanged(QHash<QString, QList<Note> >);

};

#endif // NOTEMODEL_H
