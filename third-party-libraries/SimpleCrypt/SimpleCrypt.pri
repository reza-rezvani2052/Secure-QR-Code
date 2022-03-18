#
# SimpleCrypt.pri 
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

isEmpty(SimpleCrypt_PARENTPATH) {
    message( 'SimpleCrypt_PARENTPATH is empty. use default value.' )
    SimpleCrypt_PARENTPATH = ../
} else {
    message( 'SimpleCrypt_PARENTPATH is not empty.' )
    message( $${SimpleCrypt_PARENTPATH} )
}

isEmpty(SimpleCrypt_HEADERPATH) {
    message( 'SimpleCrypt_HEADERPATH is empty. use default value.' )
    SimpleCrypt_HEADERPATH = ../SimpleCrypt/header/
} else {
    message( 'SimpleCrypt_HEADERPATH is not empty.' )
    message( $${SimpleCrypt_HEADERPATH} )
}

isEmpty(SimpleCrypt_SOURCEPATH) {
    message( 'SimpleCryptSOURCEPATH is empty. use default value.' )
    SimpleCrypt_SOURCEPATH = ../SimpleCrypt/source/
} else {
    message( 'SimpleCrypt_SOURCEPATH is not empty.' )
    message( $${SimpleCrypt_SOURCEPATH} )
}

INCLUDEPATH += .
INCLUDEPATH += $${SimpleCrypt_PARENTPATH}
INCLUDEPATH += $${SimpleCrypt_HEADERPATH}

######################################################################
# source code 

HEADERS += $${SimpleCrypt_HEADERPATH}simplecrypt.h

SOURCES += $${SimpleCrypt_SOURCEPATH}simplecrypt.cpp

######################################################################
# custom setting for compiler & system

win32-g++ {
    message("compiling for windows g++. mingw or msys or cygwin.")
    INCLUDEPATH += $${SimpleCrypt_HEADERPATH}win32-gcc 
	CONFIG(debug, debug|release) {
	} else {
	}	
}
win32-msvc2013 {
    message("compiling for visual studio 2013")
    INCLUDEPATH += $${SimpleCrypt_HEADERPATH}msvc2013 
	CONFIG(debug, debug|release) {
	} else {
	}		
}
win32-msvc2015 {
    message("compiling for visual studio 2015")
    INCLUDEPATH += $${SimpleCrypt_HEADERPATH}msvc2015 
	CONFIG(debug, debug|release) {
	} else {
	}		
}
win32-msvc2017 {
    message("compiling for visual studio 2017")
    INCLUDEPATH += $${SimpleCrypt_HEADERPATH}msvc2017 
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

   INCLUDEPATH += $${SimpleCrypt_HEADERPATH}unix-gcc

    # target.path = /usr/lib
    # INSTALLS += target
	
	CONFIG(debug, debug|release) {
	} else {
	}		
}
mac {
    message("compiling for mac os")
    INCLUDEPATH += $${SimpleCrypt_HEADERPATH}mac
	CONFIG(debug, debug|release) {
	} else {
	}		
}

