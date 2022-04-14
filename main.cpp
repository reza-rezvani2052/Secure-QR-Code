#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>
#include <QSystemTrayIcon>

#include "definitions.h"


AppInformation     AppInfo;
CompanyInformation CompanyInfo;

AppSettings appSettings;
AppDefaultSettings DefaultSettings;


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(rc);

    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);
    a.setLayoutDirection(Qt::RightToLeft);

    a.setOrganizationName(CompanyInfo.OrganizationName);
    a.setApplicationName(AppInfo.ApplicationName);
    a.setOrganizationDomain(CompanyInfo.OrganizationDomain);
    a.setApplicationVersion(AppInfo.ApplicationVersion);

    appSettings.isSystemTrayAvailable =
            QSystemTrayIcon::isSystemTrayAvailable() ? true : false;

    QApplication::setQuitOnLastWindowClosed(false);


    int id = QFontDatabase::addApplicationFont(":/fonts/iran_sans_web.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);
    font.setPointSize( qApp->font().pointSize()+1 );
    qApp->setFont(font);

    MainWindow w;
    w.show();

    return a.exec();
}
