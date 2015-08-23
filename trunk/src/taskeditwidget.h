#ifndef TASKEDITWIDGET_H
#define TASKEDITWIDGET_H

#include <QWidget>

namespace Ui {
    class TaskEditWidget;
}

class Task;

class TaskEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskEditWidget(QWidget *parent = 0);
    ~TaskEditWidget();
    Task task() const;
    void setTask(Task task);
    bool isDateValid() const;
    void clear();

private:
    Ui::TaskEditWidget *ui;

signals:
    void changed();
};

#endif // TASKEDITWIDGET_H
