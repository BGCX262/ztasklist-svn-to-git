#ifndef NOTELISTWIDGET_H
#define NOTELISTWIDGET_H

#include <QTreeView>
#include "note.h"
#include "notefiltermodel.h"

class NoteModel;

class NoteListWidget : public QTreeView
{
    Q_OBJECT
public:
    explicit NoteListWidget(QWidget *parent = 0);
    void setSourceModel(NoteModel *model);

    void appendNote(Note);
    QStringList categories();
    void removeNote(int,QModelIndex=QModelIndex());
    void replaceNote(QModelIndex,Note);
    Note getNote(QModelIndex);
    void togglePrivateNote(QModelIndex);

public slots:
    void setNameFilter(QString);
    void setPrivacyFilterState(NoteFilterModel::privacyFilterMode);


private slots:
    void currentChanged(QModelIndex current,QModelIndex last);

signals:
    void selectedItemChanged(QModelIndex);

protected:
    void mousePressEvent(QMouseEvent *e);
    void dropEvent(QDropEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);

private:
    NoteModel *noteListModel;
    NoteFilterModel *filterModel;
    QPoint startDragPos;
    void startDrag();

};

#endif // NOTELISTWIDGET_H
