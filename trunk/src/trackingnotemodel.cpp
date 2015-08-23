#include "datamanager.h"
#include "trackingnotemodel.h"

#include "programsettings.h"
TrackingNoteModel *TrackingNoteModel::getSharedModel()
{
    static TrackingNoteModel *model_=0;
    if (model_==0) model_=new TrackingNoteModel();
    return model_;
}

TrackingNoteModel::TrackingNoteModel(QObject *parent) :
    NoteModel(parent)
{
    this->setNotes(DataManager::dataManager()->notes());

    connect(DataManager::dataManager(),SIGNAL(noteListChanged(QHash<QString,QList<Note> >)),this,SLOT(setNotes(QHash<QString,QList<Note> >)));
    connect(this,SIGNAL(noteListChanged(QHash<QString,QList<Note> >)),DataManager::dataManager(),SLOT(setNotes(QHash<QString,QList<Note> >)));
}
