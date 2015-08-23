#include <QFileDialog>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

//#include <phonon/AudioOutput>
//#include <phonon/MediaSource>
#include "iconwrapper.h"
#include "another/kinotifywidget.h"

#include "programsettings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    //sound = 0;
    ui->boxRemoveQuest->setChecked(ProgramSettings::settings()->removeTaskQuestion());
    ui->pathButton->setIcon(IconWrapper::openIcon());
    ui->privatePathButton->setIcon(IconWrapper::openIcon());
    ui->backupPathButton->setIcon(IconWrapper::openIcon());
    ui->soundButton->setIcon(IconWrapper::openIcon());
    ui->pathEdit->setText(ProgramSettings::settings()->filePath());

    ui->privatePathEdit->setText(ProgramSettings::settings()->privateFilePath());
    ui->privateBox->setChecked(ProgramSettings::settings()->privateEnable());
    ui->notifBox->setValue(ProgramSettings::settings()->notifTime());
    ui->watchBox->setChecked(ProgramSettings::settings()->watchEnabled());

    ui->backupBox->setChecked(ProgramSettings::settings()->backupEnabled());
    ui->backupPathEdit->setText(ProgramSettings::settings()->backupPath());
    ui->notifDurationBox->setValue(ProgramSettings::settings()->notifDuration());
    ui->immortalBox->setChecked(ProgramSettings::settings()->immortalNotifEnabled());
    ui->exitBox->setChecked(ProgramSettings::settings()->exitQuestion());
    ui->startHiddenBox->setChecked(ProgramSettings::settings()->startHidden());
    if(ProgramSettings::settings()->buttonBarOnTop())
    {
        ui->barPosBox->setCurrentIndex(0);
    } else ui->barPosBox->setCurrentIndex(1);


    switch (ProgramSettings::settings()->notifPos())
    {
        case ProgramSettings::TopLeft: ui->topLeft->setChecked(true);break;
        case ProgramSettings::TopRight: ui->topRight->setChecked(true);break;
        case ProgramSettings::BottomLeft: ui->bottomLeft->setChecked(true);break;
        case ProgramSettings::BottomRight: ui->bottomRight->setChecked(true);break;
    }

    ui->soundBox->setChecked(ProgramSettings::settings()->notifSoundEnabled());
    if (ProgramSettings::settings()->notifSound()==":/sounds/notify.mp3")
    {
        ui->builtinButton->setChecked(true);
    } else
    {
        ui->customButton->setChecked(true);
        ui->soundEdit->setText(ProgramSettings::settings()->notifSound());
    }

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_pathButton_clicked()
{
    QString path=QFileDialog::getSaveFileName(this,tr("Select a public file"),QDir::homePath(),"ZTaskLists (*.ztl)");
    if (!path.isEmpty())
    {
        QFileInfo info(path);
        if (info.suffix()!="ztl") path+=".ztl";
        ui->pathEdit->setText(path);
    }
}

void SettingsDialog::on_buttonBox_accepted()
{
    ProgramSettings::settings()->setFilePath(ui->pathEdit->text());
    ProgramSettings::settings()->setRemoveTaskQuestion(ui->boxRemoveQuest->checkState());
    ProgramSettings::settings()->setNotifTime(ui->notifBox->value());
    ProgramSettings::settings()->setprivateEnable(ui->privateBox->isChecked());
    ProgramSettings::settings()->setPrivateFilePath(ui->privatePathEdit->text());
    ProgramSettings::settings()->setWatchEnabled(ui->watchBox->checkState());
    ProgramSettings::settings()->setBackupEnabled(ui->backupBox->isChecked());
    ProgramSettings::settings()->setBackupPath(ui->backupPathEdit->text());
    ProgramSettings::settings()->setNotifDuration(ui->notifDurationBox->value());
    ProgramSettings::settings()->setImmortalNotifEnabled(ui->immortalBox->checkState());
    ProgramSettings::settings()->setExitQuestion(ui->exitBox->isChecked());
    ProgramSettings::settings()->setStartHidden(ui->startHiddenBox->isChecked());
    if (ui->barPosBox->currentIndex()==0) ProgramSettings::settings()->setButtonBarOnTop(true); else ProgramSettings::settings()->setButtonBarOnTop(false);

    if (ui->topLeft->isChecked()) ProgramSettings::settings()->setNotifPos(ProgramSettings::TopLeft);
    if (ui->topRight->isChecked()) ProgramSettings::settings()->setNotifPos(ProgramSettings::TopRight);
    if (ui->bottomLeft->isChecked()) ProgramSettings::settings()->setNotifPos(ProgramSettings::BottomLeft);
    if (ui->bottomRight->isChecked()) ProgramSettings::settings()->setNotifPos(ProgramSettings::BottomRight);
}

void SettingsDialog::on_backupPathButton_clicked()
{
    QString path=QFileDialog::getSaveFileName(this,tr("Select a backup file"),QDir::homePath(),"ZTaskLists (*.ztl)");
    if (!path.isEmpty())
    {
        QFileInfo info(path);
        if (info.suffix()!="ztl") path+=".ztl";
        ui->backupPathEdit->setText(path);
    }
}

void SettingsDialog::on_privatePathButton_clicked()
{
    QString path=QFileDialog::getSaveFileName(this,tr("Select a private file"),QDir::homePath(),"ZTaskLists (*.ztl)");
    if (!path.isEmpty())
    {
        QFileInfo info(path);
        if (info.suffix()!="ztl") path+=".ztl";
        ui->privatePathEdit->setText(path);
    }
}


void SettingsDialog::on_pushButton_clicked()
{

    /*Phonon::MediaObject *mediaObject;
    Phonon::AudioOutput *audioOutput;
    Phonon::MediaSource source;

    audioOutput = new Phonon::AudioOutput(Phonon::NotificationCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    Phonon::createPath(mediaObject, audioOutput);
    source=Phonon::MediaSource("/home/zhulik/1.mp3");//Fix this!!!
    mediaObject->setCurrentSource(source);
    mediaObject->play();*/

    KinotifyWidget *notif=new KinotifyWidget(ui->notifDurationBox->value());

    if (ui->topLeft->isChecked()) notif->setPos(KinotifyWidget::TopLeft);;
    if (ui->topRight->isChecked()) notif->setPos(KinotifyWidget::TopRight);;
    if (ui->bottomLeft->isChecked()) notif->setPos(KinotifyWidget::BottomLeft);
    if (ui->bottomRight->isChecked()) notif->setPos(KinotifyWidget::BottomRight);

    notif->SetContent(tr("Test notification"),tr("Test notification message"),":/icons/icon");
    notif->PrepareNotification();
}

void SettingsDialog::on_soundButton_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,tr("Select a sound file"),QDir::homePath(),tr("Sounds (*.mp3 *.wav *.aac *.ogg)"));
    if (!path.isEmpty())
    {
        ui->soundEdit->setText(path);
    }
}
