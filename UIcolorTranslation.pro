#-------------------------------------------------
#
# Project created by QtCreator 2018-11-05T10:30:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# 网络请求
QT += network

# 软件exe图标
RC_FILE = softwarePicture.rc

TARGET = UIcolorTranslation
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutusdialog.cpp \
    updatedialog.cpp	\
    ftpmanager.cpp

HEADERS  += mainwindow.h \
    aboutusdialog.h \
    updatedialog.h	\
    ftpmanager.h

FORMS    += mainwindow.ui \
    aboutusdialog.ui \
    updatedialog.ui

OTHER_FILES += \
    ColorTranslate.ico

RESOURCES += \
    image.qrc
