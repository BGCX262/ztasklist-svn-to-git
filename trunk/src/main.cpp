#include <QTranslator>
#include <QMessageBox>
#include <QObject>
#include <QLocale>


#include "another/qtsingleapplication.h"
#include "mainwindow.h"
#include "programsettings.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);
    a.setApplicationVersion("2.9");
    a.setApplicationName("ztasklist");

    QString locale = QLocale::system().name();
    QTranslator *translator= new QTranslator;
    QTranslator *translatorQT= new QTranslator;
    translatorQT->load("/usr/share/qt/translations/qt_"+locale.left(2),".qm");
    translator->load(":/translations/"+locale);
    a.installTranslator(translator);
    a.installTranslator(translatorQT);

    if(a.isRunning())
    {
        QMessageBox::information(0,QObject::tr("Error!"),QObject::tr("ZTaskList already running!"));
        return 0;
    }

    MainWindow *w=new MainWindow();
    w->setVisible(!ProgramSettings::settings()->startHidden());

    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
