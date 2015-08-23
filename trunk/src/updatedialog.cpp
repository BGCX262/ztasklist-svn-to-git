#include "updatedialog.h"
#include "ui_updatedialog.h"

UpdateDialog::UpdateDialog(QString from,QString to, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    ui->setupUi(this);
    ui->label->setOpenExternalLinks(true);
    QString link=QString("<a href=\"http://gleb-zhulik.ru/ztasklist-%1.tar.gz\">Link</a>").arg(to);
    ui->label->setText(ui->label->text().arg(from).arg(to).arg(link));
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}
