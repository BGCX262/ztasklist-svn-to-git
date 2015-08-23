#include <QMessageBox>
#include "taskdialog.h"
#include "ui_taskdialog.h"
#include "ui_taskeditwidget.h"


#include "programsettings.h"

TaskDialog::TaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskDialog)
{
    ui->setupUi(this);
    this->clear();
    completed=false;
    connect(this,SIGNAL(rejected()),this,SLOT(clear()));
}

TaskDialog::~TaskDialog()
{
    delete ui;
}

Task TaskDialog::task()
{
    Task task=ui->editWidget->task();
    task.setCompleted(completed);
    clear();
    return task;
}

void TaskDialog::setTask(Task task)
{
    completed=task.completed();
    ui->editWidget->setTask(task);
}

void TaskDialog::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers()&Qt::ControlModifier)
    {
        if (e->key()==Qt::Key_Return || e->key()==Qt::Key_Enter)
        {
            this->accept();
        }
    } QDialog::keyPressEvent(e);
}

void TaskDialog::accept()
{
    bool dateError=ui->editWidget->isDateValid();
    if (dateError) QMessageBox::critical(this,tr("Date error"),tr("Selected time has passed!"));
        else QDialog::accept();
}

void TaskDialog::clear()
{
    ui->editWidget->clear();
}
