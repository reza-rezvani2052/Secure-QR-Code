#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QColor>
#include <QString>
#include <QStandardPaths>


//=================================================================================

struct CompanyInformation
{
    const QString OrganizationName   = "RezvanSoft";

#ifdef USE_LOCAL_HOST
  const QString OrganizationDomain = "http://localhost/";
#else
   const QString OrganizationDomain = "http://RezvanSoft.ir/";
#endif

};
extern CompanyInformation CompanyInfo;

//...

struct AppInformation
{
    const QString ApplicationName    = "کیوآر کد امن";
    const QString ApplicationVersion = "1.0.0";
    const QString SupportMail        = "reza.rezvani2052@gmail.com";
};
extern AppInformation AppInfo;

//...

struct AppDefaultSettings {
    const int     QRCodeBorder = 2;
    const int     QRCodeSize   = 150;
    const QColor  QRCodeColor  = Qt::black;

    const QString QRCodeFormatType = "png";
    const QString QRCodeSavePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    const uint QRCodeKey       = 10000000;      // = 10,000,000

    bool RunAtWindowsStartup    = true;
    bool ShowAtSystemTrayOnExit = true;
};
extern AppDefaultSettings DefaultSettings;

struct AppSettings {
    int     QRCodeBorder;    // = 2;
    int     QRCodeSize  ;    // = 150;
    QColor  QRCodeColor ;    // = Qt::black;

    QString QRCodeFormatType;  // = png
    QString QRCodeSavePath  ;  // = Desktop

    uint QRCodeKey      ;      // = 10,000,000

    bool RunAtWindowsStartup;    // = true
    bool ShowAtSystemTrayOnExit; // = true

    // مقادیر پیش فرض در ساختار زیر تعریف شده اند
    // AppDefaultSettings DefaultSettings;
    //
    //  تغییرات در این ساختار(به استثناء مواردی که در زیر خط چین هستند) باید در فایل
    // definitions
    // اعمال شوند

    //-------------------------------------------------

    const uint QRCodeKeyMIN = 10000001;
    const uint QRCodeKeyMAX = 999999999;

    // تعداد بار اجرای برنامه
    int numOfAppExec;

    bool isSystemTrayAvailable;
};
extern AppSettings appSettings;

//=================================================================================

enum Direction {
    TopToBottom,
    BottomToTop,
    LeftToRight,
    RightToLeft
};

//=================================================================================


#endif // DEFINITIONS_H
