#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QList>

#include "task.h"
#include "note.h"
#include "trackingtaskmodel.h"
#include "content.h"

#include "note.h"
#include "task.h"

class DataManager:public QObject
{
Q_OBJECT

public:
    static DataManager *dataManager();
    inline QList<Task> tasks()
        {return currentContent.tasks();}
    inline QHash<QString,QList<Note> > notes()
        {return currentContent.notes();}
    void refresh();

public slots:
    void setTasks(QList<Task>);
    void setNotes(QHash<QString,QList<Note> >);

signals:
    void taskListChanged(QList<Task>);
    void noteListChanged(QHash<QString,QList<Note> >);



private slots:
    void setWatchState(bool v);
    void setPath(QString);
    void setPrivatePath(QString);
    void setPrivateEnabled(bool);
    void checkUpdates();

private:

    explicit DataManager(QObject *parent = 0);

    Content importAll();
    void exportAll(Content);

    Content importFromFile(QString);
    void exportToFile(Content,QString,bool p);

    QString MD5(QString);

    void stop();
    void resume();

    Content currentContent;
    QString lastSum;
    QString lastPrivateSum;

};
#endif // TASKMANAGER_H
