#ifndef NOTEEDITDIALOG_H
#define NOTEEDITDIALOG_H

#include <QDialog>

#include "note.h"
namespace Ui {
    class NoteEditDialog;
}

class NoteEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NoteEditDialog(QWidget *parent = 0);
    ~NoteEditDialog();
    Note note();
    void setNote(Note);

private:
    Ui::NoteEditDialog *ui;
};

#endif // NOTEEDITDIALOG_H
