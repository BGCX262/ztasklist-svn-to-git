#ifndef FILTEREDIT_H
#define FILTEREDIT_H

#include <QVBoxLayout>
#include <QToolButton>
#include <QLineEdit>

class ClearLineEdit : public QLineEdit
{
Q_OBJECT
public:
    explicit ClearLineEdit(QWidget *parent = 0);
private:
    QToolButton *button;
private slots:
    void textChanged(QString);
};

#endif // FILTEREDIT_H
