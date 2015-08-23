#include "content.h"

Content::Content()
{
}

void Content::setTasks(QList<Task> newTasks)
{
    tasks_=newTasks;
}

QList<Task> Content::tasks()
{
    return tasks_;
}

void Content::setNotes(QHash <QString,QList<Note> > newNotes)
{
    notes_=newNotes;
}

QHash <QString,QList<Note> > Content::notes()
{
    return notes_;
}

Content Content::operator +(Content x)
{
    Content result;
    result.setTasks(this->tasks_+x.tasks_);
    result.setNotes(this->notes_);

    foreach (QString key,x.notes_.keys())
    {
        if (this->notes_.contains(key)) result.notes_[key].append(x.notes_[key]);
        else result.notes_.insert(key,x.notes_[key]);
    }
    return result;
}

void Content::removePublic()
{
    QList<Task> newList;
    foreach (Task task,tasks_)
    {
        if (task.isPrivate()) newList.append(task);
    }
    tasks_=newList;

    foreach(QString key,notes_.keys())
    {
        QList<Note> newList;
        foreach(Note note,notes_[key])
        {
            if (note.isPrivate()) newList.append(note);
        }
        notes_.insert(key,newList);
    }
}

void Content::removePrivate()
{
    QList<Task> newList;
    foreach (Task task,tasks_)
    {
        if (!task.isPrivate()) newList.append(task);
    }
    tasks_=newList;

    foreach(QString key,notes_.keys())
    {
        QList<Note> newList;
        foreach(Note note,notes_[key])
        {
            if (!note.isPrivate()) newList.append(note);
        }
        notes_.insert(key,newList);
    }
}

bool Content::containsTasks(QList<Task> test)
{
    int count=0;
    foreach(Task task,test)
    {
        if (tasks_.contains(task)) count++;
    }
    return count==test.count();
}

Content Content::takePublic()
{
    QList<Task> taskList;
    Content result;
    foreach (Task task,this->tasks_)
    {
        if (!task.isPrivate()) taskList.append(task);
    }
    result.tasks_=taskList;

    foreach(QString key,this->notes_.keys())
    {
        QList<Note> noteList;
        foreach(Note note,this->notes_[key])
        {
            if (!note.isPrivate()) noteList.append(note);
        }
        if (!noteList.isEmpty())
            result.notes_.insert(key,noteList);
    }
    return result;
}

Content Content::takePrivate()
{
    QList<Task> taskList;
    Content result;
    foreach (Task task,this->tasks_)
    {
        if (task.isPrivate()) taskList.append(task);
    }
    result.tasks_=taskList;

    foreach(QString key,this->notes_.keys())
    {
        QList<Note> noteList;
        foreach(Note note,this->notes_[key])
        {
            if (note.isPrivate()) noteList.append(note);
        }

        if (!noteList.isEmpty())
            result.notes_.insert(key,noteList);
    }
    return result;
}
