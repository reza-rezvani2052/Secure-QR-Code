#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(rc);

    QApplication a(argc, argv);
    a.setLayoutDirection(Qt::RightToLeft);

    a.setOrganizationName("Rezvani");
    a.setApplicationName("Secure-QR-Code");
    a.setOrganizationDomain("http://www.google.com");
    a.setApplicationVersion("1.0.0");

    int id = QFontDatabase::addApplicationFont(":/fonts/iran_sans_web.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);
    font.setPointSize( qApp->font().pointSize()+1 );
    qApp->setFont(font);

    MainWindow w;
    w.show();

    return a.exec();
}
