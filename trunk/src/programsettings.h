#ifndef PROGRAMSETTINGS_H
#define PROGRAMSETTINGS_H

#include <QObject>
//#include <phonon/mediaobject.h>

class QSettings;

class ProgramSettings : public QObject
{
Q_OBJECT
public:
    enum notifPos_ {TopLeft,TopRight,BottomLeft,BottomRight};

    static ProgramSettings *settings();

    bool removeTaskQuestion();
    void setRemoveTaskQuestion(bool state);

    QString filePath();
    void setFilePath(QString path);

    QString privateFilePath();
    void setPrivateFilePath(QString);

    QString backupPath();
    void setBackupPath(QString);

    int notifTime();
    void setNotifTime(int);

    bool privateEnable();
    void setprivateEnable(bool);

    bool watchEnabled();
    void setWatchEnabled(bool);

    bool backupEnabled();
    void setBackupEnabled(bool);

    bool buttonBarOnTop();
    void setButtonBarOnTop(bool);

    bool notifSoundEnabled();
    void setNotifSoundEnabled(bool);

    QString notifSound();
    void setNotifSound(QString);

    notifPos_ notifPos();
    void setNotifPos(notifPos_);

    int notifDuration();
    void setNotifDuration(int);

    bool immortalNotifEnabled();
    void setImmortalNotifEnabled(bool);

    bool exitQuestion();
    void setExitQuestion(bool);

    bool startHidden();
    void setStartHidden(bool);

    void showNotify(QString summary,QString body,int time=0,QString icon=QString(""),QString sound=QString(""));

private:

    explicit ProgramSettings(QObject *parent = 0);//Private contructor, singleton class

    QSettings *settings_;
    //Phonon::MediaObject *player;
    void checkFirstRun();
    QString defaultFilePath();
    QString defaultPrivateFilePath();
    QString defaultBackupFilePath();

signals:
    void pathChanged(QString);
    void privatePathChanged(QString);
    void privateTasksToggled(bool);
    void watchStateToggled(bool);
    void backupPathChanged(QString);
    void backupStateChanged(bool);
    void buttonBatOnTopStateChanged(bool);
};



#endif // PROGRAMSETTINGS_H
