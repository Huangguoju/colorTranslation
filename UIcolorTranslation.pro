#-------------------------------------------------
#
# Project created by QtCreator 2018-11-05T10:30:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# 网络请求
QT += network

TARGET = UIcolorTranslation
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutusdialog.cpp

HEADERS  += mainwindow.h \
    aboutusdialog.h

FORMS    += mainwindow.ui \
    aboutusdialog.ui

OTHER_FILES += \
    ColorTranslate.ico

RESOURCES += \
    image.qrc
