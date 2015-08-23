#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QBuffer>
#include "ui_aboutdialog.h"
#include "aboutdialog.h"
#include "updatedialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_2,SIGNAL(clicked()),qApp,SLOT(aboutQt()));
    ui->label_2->setText(ui->label_2->text().arg(qApp->applicationVersion()));
    manager=new QNetworkAccessManager(this);
    versionFile=new QBuffer(this);
    versionFile->open(QIODevice::ReadWrite);
    url="http://gleb-zhulik.ru/ztasklist.version";
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_updateButton_clicked()
{
    reply = manager->get(QNetworkRequest(url));

    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}

void AboutDialog::httpFinished()
{
    if (reply->error()!=0)
    {
        QMessageBox::information(this,tr("Error!"),reply->errorString());
        return;
    }
    float serverVersion=versionFile->buffer().trimmed().toFloat();
    if (qApp->applicationVersion().toFloat() < serverVersion)
    {
        UpdateDialog dialog(qApp->applicationVersion(),QString::number(serverVersion));
        dialog.exec();
    } else
    {
        QMessageBox::information(this,tr("No updates available"),tr("You are using the latest version."));
    }
    versionFile->close();
}

void  AboutDialog::httpReadyRead()
{
    versionFile->write(reply->readAll());
}
