#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>

#include "taskdialog.h"
#include "tasklistwidget.h"
#include "programsettings.h"
#include "taskdialog.h"
#include "note.h"

class TaskContextMenu;
class QLabel;
class TransparentButton;
class QComboBox;
class Task;
class MenuButtonAction;
class QWidgetAction;
class TimeWatcher;

class TrayIcon : public QSystemTrayIcon
{
Q_OBJECT
public:
    explicit TrayIcon(QObject *parent=0);
    ~TrayIcon();

private:

    QMenu *makeTaskMenu();
    QMenu *makeNoteMenu();
    QMenu *taskMenu;
    QMenu *noteMenu;
    TaskListWidget *taskListWidget;
    TaskContextMenu *contextMenu;
    MenuButtonAction *buttonBar;
    QWidgetAction *menuListWidget;
    TransparentButton *newTaskTButton;

protected:
    bool event(QEvent *event);

private slots:
    void updateNotes(QHash<QString,QList<Note> >);
    void newTask();
    void newNote();
    void openSettings();
    void activate(QSystemTrayIcon::ActivationReason);
    void itemDoubleClicked(QModelIndex);
    void customContextMenuRequested(const QPoint&);
    void setCurrentPriority(int);
    void removeCurrentTask();
    void togglePrivateCurrentTask();
    void toggleCurrentTask();
    void about();
    void setButtonBarOnTop(bool);
    void setCurrentRepeat(Task::RepeatType);
    void noteActionActivated();

    void mouseEnterInList();
    void mouseLeaveFromList();

    void exit();
};

#endif // TRAYICON_H
