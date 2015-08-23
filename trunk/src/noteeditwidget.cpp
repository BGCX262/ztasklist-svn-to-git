#include <QColorDialog>
#include "iconwrapper.h"
#include "noteeditwidget.h"
#include "programsettings.h"
#include "datamanager.h"
#include "trackingnotemodel.h"
#include "ui_noteeditwidget.h"

NoteEditWidget::NoteEditWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::NoteEditWidget)
{
    ui->setupUi(this);

    ui->boldButton->setIcon(IconWrapper::boldIcon());
    ui->italicButton->setIcon(IconWrapper::italicIcon());
    ui->underlineButton->setIcon(IconWrapper::underlineIcon());
    ui->colorButton->setIcon(IconWrapper::colorIcon());

    ui->createdLabel->setVisible(false);
    ui->modifiedLabel->setVisible(false);

    ui->privateBox->setVisible(ProgramSettings::settings()->privateEnable());

    noteListChanged(DataManager::dataManager()->notes());

    connect(DataManager::dataManager(),SIGNAL(noteListChanged(QHash<QString,QList<Note> >)),this,SLOT(noteListChanged(QHash<QString,QList<Note> >)));
    connect(TrackingNoteModel::getSharedModel(),SIGNAL(noteListChanged(QHash<QString,QList<Note> >)),this,SLOT(noteListChanged(QHash<QString,QList<Note> >)));

    connect(ui->boldButton,SIGNAL(clicked()),ui->textEdit,SLOT(setCurrentBold()));
    connect(ui->italicButton,SIGNAL(clicked()),ui->textEdit,SLOT(setCurrentItalic()));
    connect(ui->underlineButton,SIGNAL(clicked()),ui->textEdit,SLOT(setCurrentUnderLine()));

    connect(ui->textEdit,SIGNAL(currentBold(bool)),ui->boldButton,SLOT(setChecked(bool)));
    connect(ui->textEdit,SIGNAL(currentItalic(bool)),ui->italicButton,SLOT(setChecked(bool)));
    connect(ui->textEdit,SIGNAL(currentUnderline(bool)),ui->underlineButton,SLOT(setChecked(bool)));

    connect(ProgramSettings::settings(),SIGNAL(privateTasksToggled(bool)),ui->privateBox,SLOT(setVisible(bool)));
}

NoteEditWidget::~NoteEditWidget()
{
    delete ui;
}

Note NoteEditWidget::note()
{
    Note note;
    note.setName(ui->nameEdit->text());
    note.setText(ui->textEdit->toHtml());
    note.setCreated(created_);
    note.setModified(QDateTime::currentDateTime());
    note.setPrivate(ui->privateBox->isChecked());
    note.setCategory(ui->catBox->currentText());
    return note;
}

void NoteEditWidget::setNote(Note note)
{
    clear();
    ui->nameEdit->setText(note.name());
    ui->textEdit->setHtml(note.text());
    ui->createdLabel->setVisible(true);
    ui->modifiedLabel->setVisible(true);
    created_=note.created();
    for (int i=0;i<ui->catBox->count();++i)
    {
        if (ui->catBox->itemText(i)==note.category()) ui->catBox->setCurrentIndex(i);
    }
    ui->createdLabel->setText(tr("Created %1").arg(created_.toString("dd.MM.yy hh:mm")));
    ui->modifiedLabel->setText(tr("Modified %1").arg(note.modified().toString("dd.MM.yy hh:mm")));
    ui->privateBox->setChecked(note.isPrivate());
}

void NoteEditWidget::clear()
{
    ui->nameEdit->clear();
    ui->textEdit->clear();
    ui->boldButton->setChecked(false);//Fix this!
    ui->italicButton->setChecked(false);
    ui->underlineButton->setChecked(false);

    ui->createdLabel->setText(tr("Created %1"));
    ui->modifiedLabel->setText(tr("Modified %1"));
}

void NoteEditWidget::on_colorButton_clicked()
{
    QColor newBG=QColorDialog::getColor(Qt::white,this);
    if (newBG.isValid())
        ui->textEdit->setCurrentBackgroundColor( QColorDialog::getColor(Qt::white,this));
}

void NoteEditWidget::on_dateButton_clicked()
{
    ui->textEdit->textCursor().insertText(QDateTime::currentDateTime().toString());
}

void NoteEditWidget::setCategories(QStringList categories)
{
    ui->catBox->clear();
    ui->catBox->addItems(categories);
}

void NoteEditWidget::noteListChanged(QHash<QString,QList<Note> > notes)
{
    setCategories(notes.keys());
}
