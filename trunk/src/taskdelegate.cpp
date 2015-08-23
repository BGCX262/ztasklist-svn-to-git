#include <QPainter>
#include <QToolButton>
#include <QEvent>
#include "task.h"
#include "taskdelegate.h"

TaskDelegate::TaskDelegate(QObject *parent):QStyledItemDelegate(parent)
{

}

inline QFont getTaskFont(const QStyleOptionViewItem &option)
{
    QFont timeFont = (option.font.pointSize()>0)?option.font:QFont();
    timeFont.setPointSize(timeFont.pointSize()+4);
    timeFont.setBold(true);
    return timeFont;
}


inline QFont getDescriptionFont(const QStyleOptionViewItem &option)
{
    QFont noteFont = (option.font.pointSize()>0)?option.font:QFont();
    noteFont.setPointSize(noteFont.pointSize());
    noteFont.setItalic(true);
    return noteFont;
}

inline QFont getDateFont(const QStyleOptionViewItem &option)
{
    QFont noteFont = (option.font.pointSize()>0)?option.font:QFont();
    noteFont.setPointSize(noteFont.pointSize()+2);
    return noteFont;
}

inline QFont getTimeFont(const QStyleOptionViewItem &option)
{
    QFont noteFont = (option.font.pointSize()>0)?option.font:QFont();
    noteFont.setPointSize(noteFont.pointSize()+1);
    noteFont.setItalic(true);
    return noteFont;
}

QSize TaskDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) return QStyledItemDelegate::sizeHint(option,index);

    QSize result;
    result.setHeight(50);
    result.setWidth(330);
    return result;
}

void TaskDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    {
        Task task;
        task.setName(index.data(Task::NameRole).toString());
        task.setDescription(index.data(Task::DescriptionRole).toString());
        task.setPriority(index.data(Task::PriorityRole).toInt());
        task.setDeadlineTime(index.data(Task::DeadlineRole).toDateTime());
        task.setCompleted(index.data(Task::CompleteStatusRole).toBool());
        task.setPrivate(index.data(Task::PrivateRole).toBool());
        task.setCreateDate(index.data(Task::CreatedRole).toDate());
        task.setRepeatType((Task::RepeatType)index.data(Task::RepeatRole).toInt());

        QImage priorityImage(task.priorityIcon());




        QColor taskTextColor;
        if (option.state & QStyle::State_Selected)
        {
            if (task.deadlineTime()<QDateTime::currentDateTime() && task.deadlineTime().isValid() && !task.completed())
            {
                QColor bgColor(255,65,204);
                QBrush brush(bgColor);



                painter->fillRect(option.rect, brush);

            } else
            QStyledItemDelegate::paint(painter, option, index);
        } else if (task.deadlineTime()<QDateTime::currentDateTime() && task.deadlineTime().isValid() && !task.completed())
        {
            QColor bgColor(255,91,91);
            QBrush brush(bgColor);

            painter->fillRect(option.rect, brush);
        }

        else {
            QBrush brush = (index.row()%2)?option.palette.base():option.palette.alternateBase();
            taskTextColor.setRgb(255-brush.color().red(),
                                 255-brush.color().green(),
                                 255-brush.color().blue());

            painter->fillRect(option.rect, brush);
            }
            painter->save();
            QFont taskFont = getTaskFont(option);
            QFont descriptionFont = getDescriptionFont(option);
            QFont dateFont = getDateFont(option);
            QFont timeFont = getTimeFont(option);

            QFontMetrics fmTaskFont(taskFont);
            QFontMetrics fmDescriptionFont(descriptionFont);
            QFontMetrics fmDateFont(dateFont);
            int dateWidth=fmDateFont.width(task.deadlineTime().date().toString(Qt::SystemLocaleShortDate));

            painter->setPen(Qt::black);

            painter->setFont(taskFont);
            QRect nameRect=option.rect.adjusted(priorityImage.width(),4, -dateWidth-10,0);
            painter->drawText(nameRect,Qt::AlignHCenter, fmTaskFont.elidedText(task.name(),Qt::ElideRight,nameRect.width()-70,0));


            QRect descriptionRect=option.rect.adjusted(priorityImage.width(),30,-dateWidth-10,0);
            painter->setFont(descriptionFont);

            painter->drawText(descriptionRect,Qt::AlignHCenter,
                              fmDescriptionFont.elidedText(task.description().replace("\n"," "),Qt::ElideRight,descriptionRect.width()));


            painter->drawImage(option.rect.adjusted(0,3,-(option.rect.width()-priorityImage.width()),-(option.rect.height()-priorityImage.height())),priorityImage);
            if (task.completed()%2==1)
            {
                QImage completeImage=QImage(":/icons/task-complete");
                painter->drawImage(option.rect.adjusted(10,10,-(option.rect.width()-priorityImage.width()+10),-(option.rect.height()-priorityImage.height())-10),completeImage);
            }
            if ((int)task.repeatType()>0)
            {
                QImage completeImage=QImage(":/icons/clock");
                painter->drawImage(option.rect.adjusted(priorityImage.width(),5,-(option.rect.width()-completeImage.width()),-(option.rect.height()-completeImage.height())),completeImage);
            }

            painter->setFont(dateFont);

            painter->drawText(option.rect.adjusted((option.rect.width()-dateWidth-10),8,0,0),Qt::AlignHCenter,task.deadlineTime().date().toString(Qt::SystemLocaleShortDate));

            painter->setFont(timeFont);
            painter->drawText(option.rect.adjusted((option.rect.width()-dateWidth-10),20,0,0),Qt::AlignHCenter,task.deadlineTime().time().toString(Qt::SystemLocaleShortDate));

            if (task.isPrivate() && !task.completed())
            {
                QImage privateImage(":/icons/private");
                painter->drawImage(option.rect.adjusted(10,10,-(option.rect.width()-priorityImage.width()+10),-(option.rect.height()-priorityImage.height())-10),privateImage);
            }
            painter->restore();

        }
}
