#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QSystemTrayIcon>

#include "task.h"

namespace Ui {
    class MainWindow;
}

class TaskContextMenu;
class TrayIcon;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TrayIcon *icon;
    TaskContextMenu *contextMenu;

private slots:
    void on_noteFilterBox_currentIndexChanged(int index);
    void on_saveNoteButton_clicked();
    void on_cancelNoteButton_clicked();
    void on_actionRemove_note_triggered();
    void on_actionNew_note_triggered();
    void on_cancelTaskButton_clicked();
    void on_saveTaskButton_clicked();
    void on_actionRemove_completed_triggered();
    void on_taskFilterBox_currentIndexChanged(int index);
    void on_actionRemove_task_triggered();
    void on_actionNew_task_triggered();
    void on_tabWidget_currentChanged(int index);
    void selectedTaskChanged(QModelIndex);
    void selectedNoteChanged(QModelIndex);

    void setCurrentPriority(int p);
    void togglePrivateCurrentTask();
    void togglePrivateCurrentNote();
    void toggleCurrentTask();
    void customContextMenuRequested(const QPoint &);
    void setCurrentRepeat(Task::RepeatType);
    void activate(QSystemTrayIcon::ActivationReason reason);

    void showTasks();
    void showNotes();
    void taskListChanged(QList<Task>);

signals:

};

#endif // MAINWINDOW_H
