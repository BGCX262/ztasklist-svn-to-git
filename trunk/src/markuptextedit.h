#ifndef MARKUPTEXTEDIT_H
#define MARKUPTEXTEDIT_H

#include <QTextEdit>

class MarkupTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MarkupTextEdit(QWidget *parent = 0);

public slots:
    void setCurrentBold();
    void setCurrentItalic();
    void setCurrentUnderLine();
    void setCurrentBackgroundColor(QColor c);

private slots:
    void currentCharChanged(QTextCharFormat f);

private:
    bool isCurrentItalic;
    bool isCurrentBold;
    bool isCurrentUnderLine;
    QString eow;
    void setSelectionFormat(const QTextCharFormat &format);


protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void currentBold(bool);
    void currentItalic(bool);
    void currentUnderline(bool);

};

#endif // MARKUPTEXTEDIT_H
