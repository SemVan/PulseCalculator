#-------------------------------------------------
#
# Project created by QtCreator 2017-05-29T17:28:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = QPulseCalc
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    csvwriter.cpp \
    mathclass.cpp \
    qcustomplot.cpp \
    signalprocessing.cpp \
    polyfit.c

HEADERS  += mainwindow.h \
    csvwriter.h \
    mathclass.h \
    polyfit.h \
    qcustomplot.h \
    signalprocessing.h \
    spline.h

FORMS    += \
    mainwindow.ui



INCLUDEPATH += C:\Aquila\libs\build\include\

LIBS+=C:\Aquila\libs\build\lib\libAquila.a
LIBS+=C:\Aquila\libs\build\lib\libOoura_fft.a


QMAKE_CXXFLAGS +=-O2 -std=c++11 -Wall -Wextra


static { # everything below takes effect with CONFIG ''= static
 CONFIG+= static
 CONFIG += staticlib # this is needed if you create a static library, not a static executable
 DEFINES+= STATIC
 message("~~~ static build ~~~") # this is for information, that the static build is done
 mac: TARGET = $$join(TARGET,,,_static) #this adds an _static in the end, so you can seperate static build
#from non static build
 win32: TARGET = $$join(TARGET,,,s) #this adds an s in the end, so you can seperate static build from
#non static build
}

QMAKE_LFLAGS_RELEASE += -static -static-libgcc
