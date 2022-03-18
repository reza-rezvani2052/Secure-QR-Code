#
# qrcodegen.pri 
#

QT += core
QT += gui-private

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

isEmpty(qrcodegen_PARENTPATH) {
    message( 'qrcodegen_PARENTPATH is empty. use default value.' )
    qrcodegen_PARENTPATH = ../
} else {
    message( 'qrcodegen_PARENTPATH is not empty.' )
    message( $${qrcodegen_PARENTPATH} )
}

isEmpty(qrcodegen_HEADERPATH) {
    message( 'qrcodegen_HEADERPATH is empty. use default value.' )
    qrcodegen_HEADERPATH = ../QrCodeGen/header/
} else {
    message( 'qrcodegen_HEADERPATH is not empty.' )
    message( $${qrcodegen_HEADERPATH} )
}

isEmpty(qrcodegen_SOURCEPATH) {
    message( 'qrcodegen_SOURCEPATH is empty. use default value.' )
    qrcodegen_SOURCEPATH = ../QrCodeGen/source/
} else {
    message( 'qrcodegen_SOURCEPATH is not empty.' )
    message( $${qrcodegen_SOURCEPATH} )
}

INCLUDEPATH += .
INCLUDEPATH += $${qrcodegenPARENTPATH}
INCLUDEPATH += $${qrcodegen_HEADERPATH}

######################################################################
# source code 

HEADERS += \
$${qrcodegen_HEADERPATH}qrcodegen.h 

SOURCES += \
$${qrcodegen_SOURCEPATH}qrcodegen.cpp


######################################################################
# custom setting for compiler & system

win32-g++ {
    message("compiling for windows g++. mingw or msys or cygwin.")
    INCLUDEPATH += $${qrcodegen_HEADERPATH}win32-gcc 
	CONFIG(debug, debug|release) {
	} else {
	}	
}
win32-msvc2013 {
    message("compiling for visual studio 2013")
    INCLUDEPATH += $${qrcodegen_HEADERPATH}msvc2013 
	CONFIG(debug, debug|release) {
	} else {
	}		
}
win32-msvc2015 {
    message("compiling for visual studio 2015")
    INCLUDEPATH += $${qrcodegen_HEADERPATH}msvc2015 
	CONFIG(debug, debug|release) {
	} else {
	}		
}
win32-msvc2017 {
    message("compiling for visual studio 2017")
    INCLUDEPATH += $${qrcodegen_HEADERPATH}msvc2017 
	CONFIG(debug, debug|release) {
	} else {
	}		
}
unix {
   !contains(QT_ARCH, x86_64){
       LIB=lib32
       message("compiling for 32bit linux/unix system")
    } else {
       LIB=lib64
       message("compiling for 64bit linux/unix system")
   }

   INCLUDEPATH += $${qrcodegen_HEADERPATH}unix-gcc

    # target.path = /usr/lib
    # INSTALLS += target
	
	CONFIG(debug, debug|release) {
	} else {
	}		
}
mac {
    message("compiling for mac os")
    INCLUDEPATH += $${qrcodegen_HEADERPATH}mac
	CONFIG(debug, debug|release) {
	} else {
	}		
}

