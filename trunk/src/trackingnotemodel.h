#ifndef TRACKINGNOTEMODEL_H
#define TRACKINGNOTEMODEL_H

#include "notemodel.h"

class TrackingNoteModel : public NoteModel
{
    Q_OBJECT
public:
    static TrackingNoteModel *getSharedModel();

private:
    explicit TrackingNoteModel(QObject *parent = 0);

signals:
};

#endif // TRACKINGNOTEMODEL_H
