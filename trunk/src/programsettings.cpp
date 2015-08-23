#include <QSettings>
#include <QDir>

#include "another/kinotifywidget.h"
#include "programsettings.h"

ProgramSettings::ProgramSettings(QObject *parent):QObject(parent)
{
    settings_= new QSettings("ztasklist","ztasklist",this);
    checkFirstRun();

    //player = 0;
}

bool ProgramSettings::removeTaskQuestion()
{
    return settings_->value("removeTaskQuestion").toBool();
}

void ProgramSettings::setRemoveTaskQuestion(bool state)
{
    if (state!=removeTaskQuestion())
        settings_->setValue("removeTaskQuestion",state);
}

QString ProgramSettings::filePath()
{
    return settings_->value("filePath").toString();
}

void ProgramSettings::setFilePath(QString path)
{
    if (path!=this->filePath())
    {
        settings_->setValue("filePath",path);
        emit pathChanged(path);
    }
}

QString ProgramSettings::privateFilePath()
{
    return settings_->value("privateFilePath").toString();
}

void ProgramSettings::setPrivateFilePath(QString v)
{

    if (v!=this->privateFilePath())
    {
        settings_->setValue("privateFilePath",v);
        emit privatePathChanged(v);
    }
}

QString ProgramSettings::backupPath()
{
    return settings_->value("backupPath").toString();
}

void ProgramSettings::setBackupPath(QString v)
{
    if (v!=backupPath())
    {
        settings_->setValue("backupPath",v);
        emit backupPathChanged(v);
    }
}

int ProgramSettings::notifTime()
{
    return settings_->value("notifTime").toInt();
}

void ProgramSettings::setNotifTime(int v)
{   if (v!=notifTime())
        settings_->setValue("notifTime",v);
}

bool ProgramSettings::privateEnable()
{
    return settings_->value("privateEnable").toBool();
}



void ProgramSettings::setprivateEnable(bool v)
{
    if (v!=privateEnable())
    {
        settings_->setValue("privateEnable",v);
        emit privateTasksToggled(v);
    }
}

bool ProgramSettings::watchEnabled()
{
    return settings_->value("watchEnabled").toBool();
}

void ProgramSettings::setWatchEnabled(bool v)
{
    if (v!=watchEnabled())
    {
        settings_->setValue("watchEnabled",v);
        emit watchStateToggled(v);
    }
}

bool ProgramSettings::backupEnabled()
{
    return settings_->value("backupEnabled").toBool();
}

void ProgramSettings::setBackupEnabled(bool v)
{
    if (v!=backupEnabled())
    {
        settings_->setValue("backupEnabled",v);
        emit backupStateChanged(v);
    }
}

bool ProgramSettings::buttonBarOnTop()
{
    return settings_->value("buttonBarOnTop").toBool();
}

void ProgramSettings::setButtonBarOnTop(bool v)
{
    if (v!=buttonBarOnTop())
    {
        settings_->setValue("buttonBarOnTop",v);
        emit buttonBatOnTopStateChanged(v);
    }
}

bool ProgramSettings::notifSoundEnabled()
{
    return settings_->value("notifSoundEnabled").toBool();
}

void ProgramSettings::setNotifSoundEnabled(bool v)
{
    if (v!=notifSoundEnabled())
    {
        settings_->setValue("notifSoundEnabled",v);
    }
}

QString ProgramSettings::notifSound()
{
    return settings_->value("notifSound").toString();
}

void ProgramSettings::setNotifSound(QString v)
{
    if (v!=notifSound())
    {
        settings_->setValue("notifSound",v);
    }
}

ProgramSettings::notifPos_ ProgramSettings::notifPos()
{
    return (ProgramSettings::notifPos_ ) settings_->value("notifPos").toInt();
}

void ProgramSettings::setNotifPos(notifPos_ v)
{
    if (v!=notifPos())
    {
        settings_->setValue("notifPos",(int)v);
    }
}

int ProgramSettings::notifDuration()
{
    return settings_->value("notifDurationd").toInt();
}

void ProgramSettings::setNotifDuration(int v)
{
    if (v!=notifDuration())
    {
        settings_->setValue("notifDurationd",v);
    }
}

bool ProgramSettings::immortalNotifEnabled()
{
    return settings_->value("immortalNotifEnabled").toBool();
}

void ProgramSettings::setImmortalNotifEnabled(bool v)
{
    if (v!=immortalNotifEnabled())
    {
        settings_->setValue("immortalNotifEnabled",v);
    }
}

bool ProgramSettings::exitQuestion()
{
    return settings_->value("exitQuestion").toBool();
}

void ProgramSettings::setExitQuestion(bool v)
{
    settings_->setValue("exitQuestion",v);
}

bool ProgramSettings::startHidden()
{
    return settings_->value("startHidden").toBool();
}

void ProgramSettings::setStartHidden(bool v)
{
    settings_->setValue("startHidden",v);
}

void ProgramSettings::showNotify(QString summary,QString body,int time,QString icon,QString)
{
    /*if (player) delete player;
    if (sound.isEmpty())
        player = Phonon::createPlayer(Phonon::NotificationCategory,
                                      Phonon::MediaSource(notifSound()));
    else player = Phonon::createPlayer(Phonon::NotificationCategory,
                                       Phonon::MediaSource(sound));

    player->stop();
    player->play();*/
    KinotifyWidget *notif=new KinotifyWidget(time);
    //connect(notif,SIGNAL(destroyed()),player,SLOT(stop()));
    switch (notifPos())
    {
        case ProgramSettings::TopLeft:notif->setPos(KinotifyWidget::TopLeft);break;
        case ProgramSettings::TopRight:notif->setPos(KinotifyWidget::TopRight);break;
        case ProgramSettings::BottomLeft:notif->setPos(KinotifyWidget::BottomLeft);break;
        case ProgramSettings::BottomRight:notif->setPos(KinotifyWidget::BottomRight);break;
    }

    notif->SetContent(summary,body,icon);
    notif->PrepareNotification();
}

void ProgramSettings::checkFirstRun()
{
    if (settings_->value("removeTaskQuestion").isNull()) this->setRemoveTaskQuestion(true);
    if (settings_->value("filePath").isNull()) this->setFilePath(this->defaultFilePath());
    if (settings_->value("privateEnable").isNull()) this->setprivateEnable(false);
    if (settings_->value("watchEnabled").isNull()) this->setWatchEnabled(true);
    if (settings_->value("buttonBarOnTop").isNull()) this->setButtonBarOnTop(false);
    if (settings_->value("backupPath").isNull()) this->setBackupPath(this->defaultBackupFilePath());
    if (settings_->value("notifPos").isNull()) this->setNotifPos(BottomRight);
    if (settings_->value("privateFilePath").isNull()) this->setPrivateFilePath(this->defaultPrivateFilePath());
    if (settings_->value("notifTime").isNull()) this->setNotifTime(60);
    if (settings_->value("notifSoundEnabled").isNull()) this->setNotifSoundEnabled(true);
    if (settings_->value("immortalNotifEnabled").isNull()) this->setImmortalNotifEnabled(true);
    if (settings_->value("notifSound").isNull()) this->setNotifSound(":/sounds/notify.mp3");
    if (settings_->value("notifDurationd").isNull()) this->setNotifDuration(2000);
    if (settings_->value("exitQuestion").isNull()) this->setExitQuestion(true);
    if (settings_->value("startHidden").isNull()) this->setStartHidden(false);
}

QString ProgramSettings::defaultFilePath()
{
    return QDir::homePath()+"/.config/ztasklist/tasks.ztl";
}

QString ProgramSettings::defaultPrivateFilePath()
{
    return QDir::homePath()+"/.config/ztasklist/tasks_private.ztl";
}

QString ProgramSettings::defaultBackupFilePath()
{
    return QDir::homePath()+"/.config/ztasklist/tasks_backup.ztl";
}

ProgramSettings *ProgramSettings::settings()
{
    static ProgramSettings *settings=0;
    if (settings==0) settings=new ProgramSettings();
    return settings;
}
