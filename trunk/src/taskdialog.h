#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>

#include "task.h"

namespace Ui {
    class TaskDialog;
}

class TaskDialog: public QDialog {
    Q_OBJECT
public:
    TaskDialog(QWidget *parent = 0);
    ~TaskDialog();
    Task task();
    void setTask(Task task);

private:
    Ui::TaskDialog *ui;
    bool completed;

public slots:
    void accept();


protected:
    void keyPressEvent(QKeyEvent *);

private slots:
    void clear();
};


#endif // NEWTASKDIALOG_H
