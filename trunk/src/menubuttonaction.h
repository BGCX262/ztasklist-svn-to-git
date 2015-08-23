#ifndef MENUBUTTONACTION_H
#define MENUBUTTONACTION_H

#include <QWidgetAction>

class QToolButton;
class QHBoxLayout;

class MenuWidget : public QWidget
{
protected:
    void mouseReleaseEvent(QMouseEvent *)
    {
        //Nothing
    }
};

class MenuButtonAction : public QWidgetAction
{
    Q_OBJECT
public:
    explicit MenuButtonAction(QWidget *parent = 0);
    ~MenuButtonAction();

    void addWidget(QWidget *);
    void addSpace();

private:
    QWidget *widget;
    QHBoxLayout *layout;
    QList <QToolButton*> buttons;


};

#endif // MENUBUTTONACTION_H
