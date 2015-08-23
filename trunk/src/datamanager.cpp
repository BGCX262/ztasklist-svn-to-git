#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QHash>
#include <QCryptographicHash>

#include "datamanager.h"
#include "programsettings.h"
#include "timewatcher.h"

DataManager *DataManager::dataManager()
{
    static DataManager *manager_=0;
    if(manager_==0) manager_=new DataManager();
    return manager_;
}

DataManager::DataManager(QObject *parent) :
    QObject(parent)
{
    lastSum=MD5(ProgramSettings::settings()->filePath());
    if (ProgramSettings::settings()->privateEnable())
    {
        lastPrivateSum=MD5(ProgramSettings::settings()->privateFilePath());
    }

    resume();
    currentContent=importAll();

    connect(ProgramSettings::settings(),SIGNAL(watchStateToggled(bool)),this,SLOT(setWatchState(bool)));
    connect(ProgramSettings::settings(),SIGNAL(pathChanged(QString)),this,SLOT(setPath(QString)));
    connect(ProgramSettings::settings(),SIGNAL(privatePathChanged(QString)),this,SLOT(setPrivatePath(QString)));
    connect(ProgramSettings::settings(),SIGNAL(privateTasksToggled(bool)),this,SLOT(setPrivateEnabled(bool)));
}

void DataManager::setPath(QString path)
{
    this->stop();
    resume();

    QFile::remove(path);
    this->exportToFile(currentContent.takePublic(),path,false);
}

void DataManager::setPrivatePath(QString path)
{
    this->stop();

    resume();

    QFile::remove(path);
    this->exportToFile(currentContent.takePrivate(),path,true);
}

void DataManager::stop()
{
    disconnect(TimeWatcher::timeWatcher(),SIGNAL(secondChanged()),this,SLOT(checkUpdates()));
}

void DataManager::resume()
{
    if (ProgramSettings::settings()->watchEnabled())
    {
        lastSum=MD5(ProgramSettings::settings()->filePath());
        if (!ProgramSettings::settings()->privateEnable())
            lastPrivateSum=MD5(ProgramSettings::settings()->privateFilePath());
        connect(TimeWatcher::timeWatcher(),SIGNAL(secondChanged()),this,SLOT(checkUpdates()));
    }
}

void DataManager::checkUpdates()
{
    QString sum=MD5(ProgramSettings::settings()->filePath());
    if (lastSum!=sum)
    {
        currentContent.removePublic();
        Content imported=importFromFile(ProgramSettings::settings()->filePath());
        currentContent=currentContent+imported;
        emit taskListChanged(currentContent.tasks());
        emit noteListChanged(currentContent.notes());
        ProgramSettings::settings()->showNotify(tr("Attention!"),tr("Public task lisk updated!"),700,":/icons/icon");
        lastSum=sum;
    }
    if (!ProgramSettings::settings()->privateEnable())
    {

        sum=MD5(ProgramSettings::settings()->privateFilePath());
        if (lastPrivateSum!=sum)
        {
            currentContent.removePrivate();
            Content imported=importFromFile(ProgramSettings::settings()->privateFilePath());
            currentContent=currentContent+imported;
            emit taskListChanged(currentContent.tasks());
            emit noteListChanged(currentContent.notes());
            ProgramSettings::settings()->showNotify(tr("Attention!"),tr("Private task lisk updated!"),700,":/icons/icon");
            lastPrivateSum=sum;
        }
    }
}

void DataManager::setWatchState(bool v)
{
    if (v) resume();
    else stop();
}

void DataManager::setPrivateEnabled(bool)
{
    refresh();
}

QString DataManager::MD5(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QString data=file.readAll();
    file.close();
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(data.toUtf8());
    return md5.result().toHex().constData();
}

Content DataManager::importAll()
{
    Content Public=importFromFile(ProgramSettings::settings()->filePath());
    Content Private;
    if(ProgramSettings::settings()->privateEnable())
        Private=importFromFile(ProgramSettings::settings()->privateFilePath());

    Content Result=Public+Private;

    return Result;
}

void DataManager::exportAll(Content c)
{
    Content privateContent=c.takePrivate();
    Content publicContent=c.takePublic();
    this->stop();
    exportToFile(publicContent,ProgramSettings::settings()->filePath(),false);
    exportToFile(privateContent,ProgramSettings::settings()->privateFilePath(),true);
    this->resume();
}

void DataManager::setTasks(QList<Task> n)
{
    currentContent.setTasks(n);
    exportAll(currentContent);
}

void DataManager::setNotes(QHash<QString,QList<Note> > n)
{
    currentContent.setNotes(n);
    exportAll(currentContent);
}

void DataManager::refresh()
{
    currentContent=importAll();
    emit taskListChanged(currentContent.tasks());
    emit noteListChanged(currentContent.notes());
}

void DataManager::exportToFile(Content c,QString fileName,bool p)
{
    QDomDocument doc("ztl");
    QDomElement root=doc.createElement("ztasklist");

    QDomElement taskList=doc.createElement("tasks");
    if (p) root.setAttribute("private",true);
        foreach (Task task, c.tasks())
    {
        QDomElement taskElement=doc.createElement("task");
        taskElement.setAttribute("id",qHash(task));
        taskElement.setAttribute("name",task.name());
        taskElement.setAttribute("createDate",task.createDate().toString(Qt::ISODate));
        taskElement.setAttribute("deadline",task.deadlineTime().toString(Qt::ISODate));
        taskElement.setAttribute("priority",task.priority());
        taskElement.setAttribute("completed",QString::number(task.completed()));
        taskElement.setAttribute("repeat",QString::number((int)task.repeatType()));
        taskElement.setAttribute("notifTime",QString::number(task.notifTime()));
        taskElement.setAttribute("description",task.description());
        taskList.appendChild(taskElement);
    }
    root.appendChild(taskList);

    QDomElement noteList=doc.createElement("notes");
    foreach (QString key,c.notes().keys())
    {
        QDomElement categoryElement=doc.createElement("category");
        categoryElement.setAttribute("name",key);
        foreach (Note note,c.notes()[key])
        {
            QDomElement noteElement=doc.createElement("task");
            noteElement.setAttribute("id",qHash(note));
            noteElement.setAttribute("name",note.name());
            //noteElement.setAttribute("text",note.text());
            noteElement.setAttribute("created",note.created().toString(Qt::ISODate));
            noteElement.setAttribute("modified",note.modified().toString(Qt::ISODate));
            noteElement.setAttribute("private",note.isPrivate());
            categoryElement.appendChild(noteElement);

            QDomCDATASection description=doc.createCDATASection(note.text());
            noteElement.appendChild(description);
        }
        noteList.appendChild(categoryElement);
    }
    root.appendChild(noteList);

    doc.appendChild(root);
    QFile exportFile(fileName);
    exportFile.open(QIODevice::WriteOnly);
    QTextStream exportStream(&exportFile);
    exportStream<<doc.toString();
    exportFile.close();
}

Content DataManager::importFromFile(QString fileName)
{
    QList<Task> taskList;
    QHash<QString,QList<Note> > noteList;
    QFile importFile(fileName);
    importFile.open(QIODevice::ReadOnly);
    QDomDocument doc("ztl");
    doc.setContent(&importFile);
    importFile.close();

    QDomElement root=doc.documentElement();
    bool privateState=root.attribute("private",0).toInt();

    QDomNode tasks=root.firstChildElement("tasks");
    for(QDomElement n = tasks.firstChild().toElement(); !n.isNull(); n = n.nextSibling().toElement())
    {
        Task newTask;
        newTask.setName(n.attribute("name"));

        newTask.setCreateDate(QDate::fromString(n.attribute("createDate"),Qt::ISODate));
        newTask.setDeadlineTime(QDateTime::fromString(n.attribute("deadline"),Qt::ISODate));
        newTask.setPriority(n.attribute("priority").toInt());
        newTask.setCompleted(n.attribute("completed").toInt());
        newTask.setRepeatType((Task::RepeatType)n.attribute("repeat").toInt());
        newTask.setNotifTime(n.attribute("notifTime").toInt());
        newTask.setDescription(n.attribute("description"));


        newTask.setPrivate(privateState);
        taskList.append(newTask);
    }

    QDomNode notes=root.firstChildElement("notes");
    for(QDomElement category = notes.firstChild().toElement(); !category.isNull(); category = category.nextSibling().toElement())
    {
        QList<Note> notesInCategory;
        for(QDomElement n = category.firstChild().toElement(); !n.isNull(); n = n.nextSibling().toElement())
        {
            Note newNote;
            newNote.setName(n.attribute("name"));
            //newNote.setText(n.attribute("text"));
            newNote.setCreated(QDateTime::fromString(n.attribute("created"),Qt::ISODate));
            newNote.setModified(QDateTime::fromString(n.attribute("modified"),Qt::ISODate));
            newNote.setPrivate(privateState);
            newNote.setCategory(category.attribute("name"));

            QDomCDATASection description=n.firstChild().toCDATASection();
            newNote.setText(description.data());

            notesInCategory.append(newNote);
        }
        noteList.insert(category.attribute("name"),notesInCategory);
    }

    Content result;
    result.setTasks(taskList);
    result.setNotes(noteList);
    return result;
}
