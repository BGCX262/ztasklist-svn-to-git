#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
//#include <phonon/mediaobject.h>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_soundButton_clicked();
    void on_pushButton_clicked();
    void on_privatePathButton_clicked();
    void on_backupPathButton_clicked();
    void on_buttonBox_accepted();
    void on_pathButton_clicked();

private:
    Ui::SettingsDialog *ui;
    //Phonon::MediaObject *sound;
};

#endif // SETTINGSDIALOG_H
