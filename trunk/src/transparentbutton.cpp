#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include "transparentbutton.h"

TransparentButton::TransparentButton(QIcon icon,QWidget *parent,int size) :
    QWidget(parent)
{
    icon_=icon.pixmap(size,size).toImage();
    this->setMinimumSize(size,size);
    this->setOpacity(0.0);

    showStartState = new QState;
    showFinishState = new QState;

    enterState = new QState;

    closeStartState = new QState;
    closeFinishState = new QState;
    finalState = new QFinalState;

    QPropertyAnimation *opacityAmination = new QPropertyAnimation (this, "opacity_");
    opacityAmination->setDuration (200);

    showStartState->assignProperty (this, "opacity_", 0.0);
    showFinishState->assignProperty (this, "opacity_", 0.6);
    closeStartState->assignProperty (this, "opacity_", 0.6);
    closeFinishState->assignProperty (this, "opacity_", 0.0);
    enterState->assignProperty(this, "opacity_", 1.0);

    showStartState->addTransition (showFinishState);

    showFinishState->addTransition (this,
    SIGNAL (initiateHide ()), closeStartState);

    showFinishState->addTransition (this,
    SIGNAL (mouseEntered()), enterState);

    enterState->addTransition(this,SIGNAL(mouseLeave()),showFinishState);

    closeStartState->addTransition (closeFinishState);
    //closeFinishState->addTransition(finalState);
    closeFinishState->addTransition (closeFinishState,
    SIGNAL (propertiesAssigned ()), finalState);

    Machine_.addState (showStartState);
    Machine_.addState (showFinishState);
    Machine_.addState (enterState);
    Machine_.addState (closeStartState);
    Machine_.addState (closeFinishState);
    Machine_.addState (finalState);

    Machine_.addDefaultAnimation (opacityAmination);
    Machine_.setInitialState (showStartState);
}

TransparentButton::~TransparentButton()
{
    delete showStartState;
    delete showFinishState;

    delete enterState;

    delete closeStartState;
    delete closeFinishState;
    delete finalState;
}

void TransparentButton::show()
{
    Machine_.start();
}

void TransparentButton::hide()
{
    emit initiateHide();
    Machine_.stop();
}

void TransparentButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setOpacity(opacity_);
    painter.drawImage(0,0,icon_);
}

void TransparentButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button()==Qt::LeftButton)
    {
        if (this->rect().contains( this->mapFromGlobal(QCursor::pos())))
            emit clicked();
    }
}

void TransparentButton::mousePressEvent(QMouseEvent *)
{

}

bool TransparentButton::event(QEvent *e)
{
    if (e->type()==QEvent::Enter)
    {
        emit mouseEntered();
    }

    if (e->type()==QEvent::Leave)
    {
        emit mouseLeave();
    }
    return QWidget::event(e);
}

qreal TransparentButton::opacity() const
{
    return opacity_;
}

void TransparentButton::setOpacity(qreal v)
{
    opacity_=v;
    this->repaint();
}
