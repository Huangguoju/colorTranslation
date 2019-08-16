#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <stdlib.h>

//    qDebug() << "     " ;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //解决中文显示乱码
    //QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForCStrings(codec);
    //QTextCodec::setCodecForTr(codec);
    MainWindow w;
    w.show();
    
    return a.exec();
}
