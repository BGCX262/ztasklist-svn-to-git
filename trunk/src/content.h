#ifndef CONTENT_H
#define CONTENT_H

#include "QList"
#include "QHash"
#include "QString"

#include "task.h"
#include "note.h"

class Content
{
public:
    Content();
    void setTasks(QList<Task>);
    QList<Task> tasks();
    void setNotes(QHash <QString,QList<Note> >);
    QHash <QString,QList<Note> > notes();
    Content operator +(Content);
    void removePublic();
    void removePrivate();
    bool containsTasks(QList<Task>);

    Content takePublic();
    Content takePrivate();

private:
    QList<Task> tasks_;
    QHash <QString,QList<Note> > notes_;

};

#endif // CONTENT_H
