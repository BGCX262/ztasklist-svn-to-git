#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QUrl>

namespace Ui {
    class AboutDialog;
}

class QBuffer;
class QNetworkAccessManager;
class QNetworkReply;

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
    QNetworkAccessManager *manager;
    QUrl url;
    QBuffer *versionFile;
    QNetworkReply *reply;

private slots:
    void on_updateButton_clicked();
    void httpFinished();
    void httpReadyRead();
};

#endif // ABOUTDIALOG_H
