#ifndef TRANSPARENTBUTTON_H
#define TRANSPARENTBUTTON_H

#include <QWidget>
#include <QIcon>
#include <QStateMachine>
#include <QFinalState>


class TransparentButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY (qreal opacity_ READ opacity WRITE setOpacity);
public:
    explicit TransparentButton(QIcon,QWidget *parent = 0,int size=32);
    ~TransparentButton();

public slots:
    void show();
    void hide();

private:
    QImage icon_;
    QStateMachine Machine_;
    qreal opacity_;

    qreal opacity() const;
    void setOpacity(qreal v);

    QState *showStartState;
    QState *showFinishState;

    QState *enterState;

    QState *closeStartState;
    QState *closeFinishState;
    QFinalState *finalState;
protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    virtual bool event(QEvent *);


signals:
    void clicked();
    void initiateHide ();
    void mouseEntered();
    void mouseLeave();


};

#endif // TRANSPARENTBUTTON_H
