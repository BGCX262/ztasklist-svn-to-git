#include <QToolButton>
#include <QHBoxLayout>
#include "menubuttonaction.h"

MenuButtonAction::MenuButtonAction(QWidget *parent) :
    QWidgetAction(parent)
{
    widget=new MenuWidget();
    this->setDefaultWidget(widget);
    layout=new QHBoxLayout(widget);
    layout->setSpacing(0);
    layout->setMargin(0);
}

MenuButtonAction::~MenuButtonAction()
{
    delete widget;
}

void MenuButtonAction::addWidget(QWidget *widget)
{
    QToolButton *button=qobject_cast<QToolButton*>(widget);
    if (button)
        button->setIconSize(QSize(24,24));
    layout->addWidget(widget);
}

void MenuButtonAction::addSpace()
{
    layout->addStretch();
}
