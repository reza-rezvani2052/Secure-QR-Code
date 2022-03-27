QT       += core widgets gui svg

CONFIG += c++11

#-----------------------------------------------------------------------

# qrcodegen code for Application Qt project
qrcodegen_PARENTPATH=./third-party-libraries/QrCodeGen/
qrcodegen_HEADERPATH=./third-party-libraries/QrCodeGen/header/  # current qrcodegen path is ./header/
qrcodegen_SOURCEPATH=./third-party-libraries/QrCodeGen/source/  # current qrcodegen path is ./source/
include(./third-party-libraries/QrCodeGen/qrcodegen.pri)

#...

# SimpleCrypt code for Application Qt project
SimpleCrypt_PARENTPATH=./third-party-libraries/SimpleCrypt/
SimpleCrypt_HEADERPATH=./third-party-libraries/SimpleCrypt/header/
SimpleCrypt_SOURCEPATH=./third-party-libraries/SimpleCrypt/source/
include(./third-party-libraries/SimpleCrypt/SimpleCrypt.pri)

#-----------------------------------------------------------------------

DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    formchangekey.cpp \
    formsetting.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    definitions.h \
    formchangekey.h \
    formsetting.h \
    mainwindow.h

FORMS += \
    formchangekey.ui \
    formsetting.ui \
    mainwindow.ui

#مسیر فایل اجرایی
DESTDIR = $$PWD/bin


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rc/rc.qrc

win32{
    RC_FILE = rc/win-rc.rc
#   message( "hmmmmmm" )
#   message( PWD = $$PWD )
}unix{
    error("RC FILE is not set")
}macx{
    error("RC FILE is not set")
}
