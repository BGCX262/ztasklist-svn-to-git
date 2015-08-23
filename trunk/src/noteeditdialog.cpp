#include "noteeditdialog.h"
#include "ui_noteeditdialog.h"

NoteEditDialog::NoteEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoteEditDialog)
{
    ui->setupUi(this);
}

NoteEditDialog::~NoteEditDialog()
{
    delete ui;
}


Note NoteEditDialog::note()
{
    return ui->widget->note();
}

void NoteEditDialog::setNote(Note note)
{
    ui->widget->setNote(note);
}
