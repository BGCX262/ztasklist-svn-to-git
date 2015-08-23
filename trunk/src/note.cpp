#include <QString>
#include <QHash>

#include "note.h"

Note::Note()
{
    private_=false;
}

Note::Note(const Note &x)
{
    this->name_=x.name_;
    this->text_=x.text_;
    this->created_=x.created_;
    this->modified_=x.modified_;
    this->private_=x.private_;
    this->category_=x.category_;
}

void Note::setName(QString newName)
{
    name_=newName;
}

void Note::setText(QString newText)
{
    text_=newText;
}

void Note::setCreated(QDateTime newTime)
{
    created_=newTime;
}

void Note::setModified(QDateTime newTime)
{
    modified_=newTime;
}

void Note::setPrivate(bool v)
{
    private_=v;
}

void Note::setCategory(QString c)
{
    category_=c;
}

void Note::operator=(Note x)
{
    this->name_=x.name_;
    this->text_=x.text_;
    this->created_=x.created_;
    this->modified_=x.modified_;
    this->private_=x.private_;
    this->category_=x.category_;
}

bool Note::operator==(Note x) const
{
    return (this->name_==x.name_ &&
            this->text_==x.text_ &&
            this->category_==x.category_ &&
            this->private_==x.private_ &&
            this->created_==x.created() &&
            this->modified_==x.modified_);
}

bool Note::operator<(const Note x) const
{
    return this->created_>x.created_;
}

uint qHash(const Note &note)
{
   return  qHash(note.name()+"|"+
           note.text()+"|"+
           note.created().toString()+"|"+
           note.modified().toString()+"|"+
           note.isPrivate() +"|"+
           note.category());
}

QDataStream &operator<<(QDataStream &stream, const Note &note)
{
    stream<<note.name();
    stream<<note.text();
    stream<<note.created();
    stream<<note.modified();
    stream<<note.isPrivate();
    stream<<note.category();
    return stream;
}

QDataStream &operator>>(QDataStream &stream,Note &note)
{
    QString tempS;
    bool tempB;
    QDateTime tempDT;

    stream>>tempS; note.setName(tempS);
    stream>>tempS; note.setText(tempS);
    stream>>tempDT; note.setCreated(tempDT);
    stream>>tempDT; note.setModified(tempDT);
    stream>>tempB; note.setPrivate(tempB);
    stream>>tempS; note.setCategory(tempS);
    return stream;
}
