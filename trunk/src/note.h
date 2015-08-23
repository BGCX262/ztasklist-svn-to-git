#ifndef NOTE_H
#define NOTE_H

#include <QDateTime>
#include <QMetaType>
class QString;

class Note
{
public:
    Note();
    Note(const Note &);

    inline QString name() const
        {return name_;}
    void setName(QString);

    inline QString text() const
        {return text_;}
    void setText(QString);

    inline QDateTime created() const
        {return created_;}
    void setCreated(QDateTime);

    inline QDateTime modified() const
        {return modified_;}
    void setModified(QDateTime);

    inline bool isPrivate() const
        {return private_;}
    void setPrivate(bool);

    inline QString category() const
        {return category_;}
    void setCategory(QString);

    void operator=(Note);
    bool operator<(const Note) const;
    bool operator==(Note) const;

private:
    QString name_;
    QString text_;
    QDateTime created_;
    QDateTime modified_;
    bool private_;
    QString category_;
};
Q_DECLARE_METATYPE(Note);

QDataStream &operator<<(QDataStream &, const Note &);
QDataStream &operator>>(QDataStream &,Note &);

uint qHash(const Note &note);

#endif // NOTE_H
