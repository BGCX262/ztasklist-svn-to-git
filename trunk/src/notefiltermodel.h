#ifndef NOTEFILTERMODEL_H
#define NOTEFILTERMODEL_H

#include <QSortFilterProxyModel>

#include "notemodel.h"

class NoteFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    enum privacyFilterMode{all,onlyPublic,onlyPrivate};
    explicit NoteFilterModel(QObject *parent=0);
    void setSourceModel(NoteModel *sourceModel);

    void setNameFilter(QString);
    void setPrivacyFilterMode(privacyFilterMode);

signals:
    void noteListChanged(QHash<QString,QList<Note> >);
private:
    bool filterAcceptsRow(int row, const QModelIndex &) const;

    QString nameFilter_;
    NoteModel *sourceModel_;
    privacyFilterMode privacyFilterMode_;
};

#endif // NOTEFILTERMODEL_H
