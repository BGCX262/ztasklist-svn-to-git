#include "task.h"
#include "taskeditwidget.h"
#include "ui_taskeditwidget.h"

#include "programsettings.h"

TaskEditWidget::TaskEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskEditWidget)
{
    ui->setupUi(this);

    connect(ProgramSettings::settings(),SIGNAL(privateTasksToggled(bool)),ui->privateBox,SLOT(setVisible(bool)));
}

TaskEditWidget::~TaskEditWidget()
{
    delete ui;
}

Task TaskEditWidget::task()const
{
    Task task;
    task.setPriority(ui->comboBox->currentIndex());
    task.setName(ui->lineEdit->text().trimmed());
    task.setDescription(ui->plainTextEdit->toPlainText().trimmed());
    task.setNotifTime(ui->notifBox->value());
    task.setPrivate(ui->privateBox->checkState());

    switch(ui->repeatBox->currentIndex())
    {
        case 0: task.setRepeatType(Task::NoRepeat); break;
        case 1: task.setRepeatType(Task::DayRepeat); break;
        case 2: task.setRepeatType(Task::WeekRepeat);break;
        case 3: task.setRepeatType(Task::monthRepeat); break;
    }

    if (ui->groupBox->isChecked())
    {
        QDateTime temp;
        temp.setDate(ui->dateEdit->date());

        QTime time=ui->timeEdit->time();
        temp.setTime(QTime(time.hour(),time.minute(),0));
        task.setDeadlineTime(temp);

    }
    else task.setDeadlineTime(QDateTime());
    return task;
}

void TaskEditWidget::setTask(Task task)
{
    this->clear();
    ui->comboBox->setCurrentIndex(task.priority());
    ui->lineEdit->setText(task.name());
    ui->plainTextEdit->setPlainText(task.description());
    ui->privateBox->setChecked(task.isPrivate());
    ui->createdLabel->setText(tr("Created %1").arg(task.createDate().toString("dd.MM.yyyy")));
    ui->createdLabel->setVisible(!task.createDate().toString().isEmpty());

    switch(task.repeatType())
    {
        case Task::NoRepeat: ui->repeatBox->setCurrentIndex(0);break;
        case Task::DayRepeat: ui->repeatBox->setCurrentIndex(1);break;
        case Task::WeekRepeat: ui->repeatBox->setCurrentIndex(2);break;
        case Task::monthRepeat: ui->repeatBox->setCurrentIndex(3);break;
    }

    if (task.deadlineTime().isValid())
    {
        ui->groupBox->setChecked(true);
        ui->dateEdit->setDate(task.deadlineTime().date());
        ui->timeEdit->setTime(task.deadlineTime().time());
        ui->notifBox->setValue(task.notifTime());
    }


}

bool TaskEditWidget::isDateValid() const
{
    bool dateError=false;
    if (ui->groupBox->isChecked())
    {
        if (ui->dateEdit->date()==QDate::currentDate())
        {
            if (ui->timeEdit->time()<QTime::currentTime())
                dateError=true;
        } else
        if (ui->dateEdit->date()<QDate::currentDate())
        {
            dateError=true;
        }
    }
    return dateError;
}

void TaskEditWidget::clear()
{
    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit->setTime(QTime::currentTime());
    ui->createdLabel->setVisible(false);

    ui->lineEdit->clear();
    ui->plainTextEdit->clear();
    ui->comboBox->setCurrentIndex(1);
    ui->privateBox->setVisible(ProgramSettings::settings()->privateEnable());
    ui->repeatBox->setCurrentIndex(0);
    ui->groupBox->setChecked(false);
    ui->notifBox->setValue(0);
    ui->privateBox->setChecked(false);
}

