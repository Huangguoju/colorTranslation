#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDragEnterEvent>
#include <QDebug>
#include <QGraphicsView>
#include <QFileDialog>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug() << "sdgsss2222222222";
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    ui->comboBox->addItem("VIDEO_NORMAL", 0);
    ui->comboBox->addItem("VIDEO_720P", 1);
    ui->comboBox->addItem("VIDEO_720P_T", 2);
    ui->comboBox->addItem("VIDEO_720P_C", 3);
    ui->comboBox->addItem("VIDEO_1080P", 4);
    ui->comboBox->addItem("VIDEO_1080P_T", 5);
    ui->comboBox->addItem("VIDEO_1080P_C", 6);
    ui->comboBox->addItem("VIDEO_3M_A", 7);
    ui->comboBox->addItem("VIDEO_3M_T", 8);

    QLine_UIdefault_list << ui->default_line1 << ui->default_line2 << ui->default_line3
                            << ui->default_line4 << ui->default_line5;
    QLine_UIcolor_list << ui->UIcolor_line1 << ui->UIcolor_line2 << ui->UIcolor_line3
                            << ui->UIcolor_line4 << ui->UIcolor_line5;
    QLine_TRcolor_list << ui->TRcolor_line1 << ui->TRcolor_line2 << ui->TRcolor_line3
                            << ui->TRcolor_line4 << ui->TRcolor_line5;
    QLine_Cameracolor_list << ui->CameraColor_line1 << ui->CameraColor_line2 << ui->CameraColor_line3
                            << ui->CameraColor_line4 << ui->CameraColor_line5;

}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "asdgsadgh";
    //QFileDialog *file = new QFileDialog;
    //file->Accept();
}

void MainWindow::on_commandLinkButton_clicked()
{

    for(int i = 0; i < 5; i++)
    {
        RS_S32 uiColorVal = QLine_UIcolor_list[i]->text().toInt();
        RS_S32 colorVal = translate(i, uiColorVal);
        qDebug() << "colorVal:" << colorVal;
        QString TRcolor = QString("%1").arg(colorVal);
        QLine_TRcolor_list[i]->setText(TRcolor);


    }


}

RS_S32 MainWindow::translate(int colorType,RS_S32 uiColorVal)
{
#if 1
    RS_S32 maxColorVal = 63;
    RS_S32 colorVal = 0;
    RS_S32 UIColorDefaultVal = QLine_UIdefault_list[colorType]->text().toInt();
    RS_S32 CameraColorVal = QLine_Cameracolor_list[colorType]->text().toInt();
    //if(cfg_get_colorMode() == 1)
        maxColorVal = 255;

    if(uiColorVal >= 0 && uiColorVal <= maxColorVal)
    {



        if(uiColorVal == UIColorDefaultVal)
        {
            colorVal = CameraColorVal;
        }
        else if(uiColorVal > UIColorDefaultVal)
        {
            colorVal = (int)((uiColorVal*(maxColorVal - CameraColorVal) + maxColorVal*(CameraColorVal - \
                UIColorDefaultVal))/(maxColorVal - UIColorDefaultVal));
        }
        else if(uiColorVal < UIColorDefaultVal)
        {
            colorVal = (int)((uiColorVal*CameraColorVal)/UIColorDefaultVal);
        }

    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("uiColorVal is err!");
        //msgBox.setInformativeText("Do you want to save your changes?");
        //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        //msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.exec();
        //DEBUG_INFO(("uiColorVal is err!\n"));
        return maxColorVal;
    }
    return colorVal;

   #endif
}






