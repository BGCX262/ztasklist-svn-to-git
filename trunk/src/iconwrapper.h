#ifndef ICONWRAPPER_H
#define ICONWRAPPER_H

#include <QIcon>

namespace IconWrapper
{
    inline  QIcon completeIcon()
    {
        return QIcon::fromTheme("task-complete",QIcon(":/icons/task-complete"));
    }

    inline  QIcon addIcon()
    {
        return QIcon::fromTheme("list-add",QIcon(":/icons/list-add"));
    }

    inline  QIcon removeIcon()
    {
        return QIcon::fromTheme("list-remove",QIcon(":/icons/remove"));
    }

    inline  QIcon prefIcon()
    {
        return QIcon::fromTheme("preferences-system",QIcon(":/icons/remove"));
    }

    inline  QIcon exitIcon()
    {
        return QIcon::fromTheme("application-exit", QIcon(":/icons/application-exit"));
    }

    inline  QIcon openIcon()
    {
        return QIcon::fromTheme("document-open",QIcon(":/icons/document-open"));
    }

    inline  QIcon allIcon()
    {
        return QIcon::fromTheme("mail-mark-task",QIcon(":/icons/all-tasks"));
    }

    inline  QIcon importIcon()
    {
        return QIcon::fromTheme("document-import",QIcon(":/icons/import"));
    }

    inline  QIcon exportIcon()
    {
        return QIcon::fromTheme("document-export",QIcon(":/icons/export"));
    }

    inline  QIcon clearIcon()
    {
        return QIcon::fromTheme("edit-clear",QIcon(":/icons/clear"));
    }

    inline  QIcon editClearIcon()
    {
        return QIcon::fromTheme("edit-clear-locationbar-rtl",QIcon(":/icons/edit-clear"));
    }

    inline  QIcon syncIcon()
    {
        return QIcon::fromTheme("reload",QIcon(":/icons/sync"));
    }

    inline  QIcon privateIcon()
    {
        return QIcon::fromTheme("object-locked",QIcon(":/icons/private"));
    }

    inline  QIcon aboutIcon()
    {
        return QIcon::fromTheme("help-about",QIcon(":/icons/about"));
    }

    inline  QIcon filterIcon()
    {
        return QIcon::fromTheme("view-filter",QIcon(":/icons/filter"));
    }

    inline  QIcon backupIcon()
    {
        return QIcon(":/icons/backup");
    }

    inline  QIcon boldIcon()
    {
        return QIcon::fromTheme("format-text-bold",QIcon(":/markup/bold"));
    }

    inline  QIcon italicIcon()
    {
        return QIcon::fromTheme("format-text-italic",QIcon(":/markup/italic"));
    }

    inline  QIcon underlineIcon()
    {
        return QIcon::fromTheme("format-text-underline",QIcon(":/markup/underline"));
    }

    inline  QIcon colorIcon()
    {
        return QIcon::fromTheme("format-text-color",QIcon(":/markup/color"));
    }
};

#endif // ICONWRAPPER_H
