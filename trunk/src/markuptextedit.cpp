#include <QKeyEvent>
#include "markuptextedit.h"

inline QTextCharFormat defaultFormat()
{
    QTextCharFormat curFormat;
    curFormat.setFontWeight(QFont::Normal);
    curFormat.setFontItalic(false);
    curFormat.setFontUnderline(false);
    curFormat.setFontPointSize(10);
    curFormat.setForeground(Qt::black);
    curFormat.setBackground(Qt::white);
    return curFormat;
}

MarkupTextEdit::MarkupTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    isCurrentItalic=false;
    isCurrentBold=false;
    isCurrentUnderLine=false;
    mergeCurrentCharFormat(defaultFormat());
    eow="~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=\n ";
    connect(this,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentCharChanged(QTextCharFormat)));
}



void MarkupTextEdit::setCurrentBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(!isCurrentBold ? QFont::Bold : QFont::Normal);
    setSelectionFormat(fmt);
}

void MarkupTextEdit::setCurrentItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(!isCurrentItalic);
    setSelectionFormat(fmt);
}

void MarkupTextEdit::setCurrentUnderLine()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(!isCurrentUnderLine);
    setSelectionFormat(fmt);
}

void MarkupTextEdit::setSelectionFormat(const QTextCharFormat &format)
{
    QTextCursor cursor = this->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    this->mergeCurrentCharFormat(format);
}

void MarkupTextEdit::currentCharChanged(QTextCharFormat f)
{
    isCurrentBold=f.font().bold();
    isCurrentUnderLine=f.font().underline();
    isCurrentItalic=f.font().italic();

    emit currentBold(isCurrentBold);
    emit currentItalic(isCurrentItalic);
    emit currentUnderline(isCurrentUnderLine);
}

void MarkupTextEdit::setCurrentBackgroundColor(QColor c)
{
    QTextCharFormat fmt;
    fmt.setBackground(QBrush(c));
    setSelectionFormat(fmt);
}

void MarkupTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (eow.contains(e->text()))
    {
        mergeCurrentCharFormat(defaultFormat());
    }

    QTextEdit::keyPressEvent(e);
}
