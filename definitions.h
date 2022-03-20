#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QColor>
#include <QString>

//=================================================================================

struct CompanyInformation
{
    const QString OrganizationName   = "HosFa";

#ifdef USE_LOCAL_HOST
  const QString OrganizationDomain = "http://localhost/";
#else
   const QString OrganizationDomain = "http://HosFa.ir/";
#endif

};
extern CompanyInformation CompanyInfo;

//...

struct AppInformation
{
    const QString ApplicationName    = "Secure-QR-Code";
    const QString ApplicationVersion = "1.0.0";
    const QString SupportMail        = "hosfa@gmail.com";
};
extern AppInformation AppInfo;

//...

struct AppDefaultSettings {
    const int     QRCodeBorder = 2;
    const int     QRCodeSize   = 150;
    const QColor  QRCodeColor  = Qt::black;
};
extern AppDefaultSettings DefaultSettings;

//=================================================================================


#endif // DEFINITIONS_H
