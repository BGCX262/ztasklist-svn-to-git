#ifndef NOTEEDITWIDGET_H
#define NOTEEDITWIDGET_H

#include <QWidget>
#include <QHash>
#include <QDateTime>

#include "note.h"

namespace Ui {
    class NoteEditWidget;
}

class NoteEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NoteEditWidget(QWidget *parent = 0);
    ~NoteEditWidget();

    Note note();
    void setNote(Note);

    void setCategories(QStringList);
    void clear();

private:
    Ui::NoteEditWidget *ui;
    QDateTime created_;


private slots:
    void noteListChanged(QHash<QString,QList<Note> >);

private slots:
    void on_dateButton_clicked();
    void on_colorButton_clicked();
};

#endif // NOTEEDITWIDGET_H
