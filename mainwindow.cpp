#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::init()
{
    ui->cameraFormatBox->addItem("VIDEO_NORMAL", 0);
    ui->cameraFormatBox->addItem("VIDEO_720P", 1);
    ui->cameraFormatBox->addItem("VIDEO_720P_T", 2);
    ui->cameraFormatBox->addItem("VIDEO_720P_C", 3);
    ui->cameraFormatBox->addItem("VIDEO_1080P", 4);
    ui->cameraFormatBox->addItem("VIDEO_1080P_T", 5);
    ui->cameraFormatBox->addItem("VIDEO_1080P_C", 6);
    ui->cameraFormatBox->addItem("VIDEO_3M_A", 7);
    ui->cameraFormatBox->addItem("VIDEO_3M_T", 8);
    ui->cameraFormatBox->addItem("VIDEO_3M_C", 9);
    ui->cameraFormatBox->addItem("VIDEO_QXGA_A", 10);
    ui->cameraFormatBox->addItem("VIDEO_QXGA_T", 11);
    ui->cameraFormatBox->addItem("VIDEO_4M_A", 12);
    ui->cameraFormatBox->addItem("VIDEO_4M_T", 13);
    ui->cameraFormatBox->addItem("VIDEO_4M_C", 14);
    ui->cameraFormatBox->addItem("VIDEO_5M_A", 15);
    ui->cameraFormatBox->addItem("VIDEO_5M_T", 16);
    ui->cameraFormatBox->addItem("VIDEO_8M_A", 17);
    ui->cameraFormatBox->addItem("VIDEO_8M_T", 18);
    ui->cameraFormatBox->addItem("VIDEO_8M_C", 19);
    ui->cameraFormatBox->setCurrentIndex(4);

    ui->ADchipTypeBox->addItem("NORMAL", 1);
    ui->ADchipTypeBox->addItem("Color_AHD_6124B", 2);
    ui->ADchipTypeBox->addItem("Color_TVI_2823/2853", 3);
    ui->ADchipTypeBox->addItem("Color_AHD_6134", 7);
    ui->ADchipTypeBox->addItem("Color_AHD_6134C", 8);
    ui->ADchipTypeBox->addItem("Color_TVI_2826", 9);
    ui->ADchipTypeBox->addItem("Color_TVI_2827", 10);
    ui->ADchipTypeBox->addItem("Color_TVI_2827C", 11);
    ui->ADchipTypeBox->addItem("Color_AHD_6158", 12);
    ui->ADchipTypeBox->addItem("Color_AHD_6158C", 13);
    ui->ADchipTypeBox->addItem("Color_TVI_2829", 14);
    ui->ADchipTypeBox->addItem("Color_TVI_2828", 15);
    ui->ADchipTypeBox->setCurrentIndex(3);

    QLine_UIcolor_list << ui->UIcolor_line1 << ui->UIcolor_line2 << ui->UIcolor_line3
                            << ui->UIcolor_line4 << ui->UIcolor_line5;
    QLine_TRcolor_list << ui->TRcolor_line1 << ui->TRcolor_line2 << ui->TRcolor_line3
                            << ui->TRcolor_line4 << ui->TRcolor_line5;

    ui->UIDefaultDisplayVal->clear();
    ui->CameraDefaultMapVal->clear();

}

int MainWindow::ReadColorSetting(const QSettings &config, const QString &key,
                     RS_U8 *pColorSettting)
{
    QString str = config.value(key).toString();
    QString strTrimed;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ',' || (str[i] >= '0' && str[i] <= '9'))
            strTrimed.append(str[i]);
    }

    QStringList strList = strTrimed.split(',');
    if (strList.size() == 5)
    {
        for(int i = 0; i < strList.size(); i++)
        {
            pColorSettting[i] = strList[i].toUInt();
        }
        return 0;
    }
    else
    {
        for(int i = 0; i < strList.size(); i++)
        {
            pColorSettting[i] = 0;
        }
        return -1;
    }
}

void MainWindow::ConfigCameraColor(int chiptype)
{
    //QSettings config("custom-config.ini", QSettings::IniFormat);
    QSettings config(filename, QSettings::IniFormat);
    QString maindirect;
    for(int i = 0; i < VIDEO_MAX; i++){
        memset(CameraColorVal[i], 0, 5);
    }

    switch(chiptype)  {
        case RS_AD_CHIP_TYPE_AHD:
            maindirect = "Color_AHD_6124B";
            break;
        case RS_AD_CHIP_TYPE_AHD_6134C:
            maindirect = "Color_AHD_6134C";
            break;
        case RS_AD_CHIP_TYPE_TVI:
            maindirect = "Color_TVI_2823/2853";
            break;
        case RS_AD_CHIP_TYPE_AHD_6134:
            maindirect = "Color_AHD_6134";
            break;
        case RS_AD_CHIP_TYPE_TVI_2826:
            maindirect = "Color_TVI_2826";
            break;
        case RS_AD_CHIP_TYPE_TVI_2827:
            maindirect = "Color_TVI_2827";
            break;
        case RS_AD_CHIP_TYPE_TVI_2827C:
            maindirect = "Color_TVI_2827C";
            break;
        case RS_AD_CHIP_TYPE_AHD_6158:
            maindirect = "Color_AHD_6158";
            break;
        case RS_AD_CHIP_TYPE_AHD_6158C:
            maindirect = "Color_AHD_6158C";
            break;
        case RS_AD_CHIP_TYPE_TVI_2829:
            maindirect = "Color_TVI_2829";
            break;
        case RS_AD_CHIP_TYPE_TVI_2828:
            maindirect = "Color_TVI_2828";
            break;
        default:
            maindirect = "Color_NORMAL";
            break;
    }

    ReadColorSetting(config, maindirect + "/NormalColorDefaultVal", CameraColorVal[VIDEO_NORMAL]);
    ReadColorSetting(config, maindirect + "/AHD720PColorDefaultVal", CameraColorVal[VIDEO_720P]);
    ReadColorSetting(config, maindirect + "/AHD1080PColorDefaultVal", CameraColorVal[VIDEO_1080P]);
    ReadColorSetting(config, maindirect + "/AHD300WColorDefaultVal", CameraColorVal[VIDEO_QXGA_A]);
    ReadColorSetting(config, maindirect + "/AHD400WColorDefaultVal", CameraColorVal[VIDEO_4M_A]);
    ReadColorSetting(config, maindirect + "/AHD500WColorDefaultVal", CameraColorVal[VIDEO_5M_A]);
    ReadColorSetting(config, maindirect + "/AHD800WColorDefaultVal", CameraColorVal[VIDEO_8M_A]);
    ReadColorSetting(config, maindirect + "/TVI720PColorDefaultVal", CameraColorVal[VIDEO_720P_T]);
    ReadColorSetting(config, maindirect + "/TVI1080PColorDefaultVal", CameraColorVal[VIDEO_1080P_T]);
    ReadColorSetting(config, maindirect + "/TVI300WColorDefaultVal", CameraColorVal[VIDEO_3M_T]);
    ReadColorSetting(config, maindirect + "/TVI400WColorDefaultVal", CameraColorVal[VIDEO_4M_T]);
    ReadColorSetting(config, maindirect + "/TVI500WColorDefaultVal", CameraColorVal[VIDEO_5M_T]);
    ReadColorSetting(config, maindirect + "/TVI800WColorDefaultVal", CameraColorVal[VIDEO_8M_T]);
    ReadColorSetting(config, maindirect + "/CVI720PColorDefaultVal", CameraColorVal[VIDEO_720P_C]);
    ReadColorSetting(config, maindirect + "/CVI1080PColorDefaultVal", CameraColorVal[VIDEO_1080P_C]);
    ReadColorSetting(config, maindirect + "/CVI300WColorDefaultVal", CameraColorVal[VIDEO_3M_C]);
    ReadColorSetting(config, maindirect + "/CVI400WColorDefaultVal", CameraColorVal[VIDEO_4M_C]);
    ReadColorSetting(config, maindirect + "/CVI800WColorDefaultVal", CameraColorVal[VIDEO_8M_C]);

    ReadColorSetting(config, "DisplaySenceDef/UIColorDefaultVal", UIColorDefaultVal);
}

void MainWindow::on_openFile_clicked()
{

    filename = QFileDialog::getOpenFileName(this,tr("open custom-config.ini"),
                                            QDir::currentPath(), tr("configuration files (*.ini)"));

    if (!filename.isEmpty()) {
        if (ui->directoryComboBox->findText(filename) == -1)
            ui->directoryComboBox->addItem(filename);
        ui->directoryComboBox->setCurrentIndex(ui->directoryComboBox->findText(filename));
    }

    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return;

//    QTextStream in(&file);
//    while (!in.atEnd()) {
//        QString line = in.readLine();
//        qDebug() << line ;
//    }
    QSettings config(filename, QSettings::IniFormat);
//    RS_U8 UIColorDefaultVal[5];
//    ReadColorSetting(config, "DisplaySenceDef/UIColorDefaultVal", UIColorDefaultVal);
//    qDebug() << UIColorDefaultVal[0] << UIColorDefaultVal[1];

    ColorMode = config.value("DisplaySenceDef/ColorMode").toUInt();

    QString m_DefaultDisplayVal = config.value("DisplaySenceDef/UIColorDefaultVal").toString();
    ui->UIDefaultDisplayVal->setText(m_DefaultDisplayVal);

    doRefreshCameraDefaultMapVal();
}

void MainWindow::doRefreshCameraDefaultMapVal()
{
    int m_ADType = ui->ADchipTypeBox->itemData(ui->ADchipTypeBox->currentIndex()).toInt();

    ConfigCameraColor(m_ADType);

    int m_videoFormat = ui->cameraFormatBox->currentIndex();

    char CameraTmp[16];
    sprintf(CameraTmp, "hbcss{%d, %d, %d, %d, %d}", CameraColorVal[m_videoFormat][0], CameraColorVal[m_videoFormat][1],\
            CameraColorVal[m_videoFormat][2],CameraColorVal[m_videoFormat][3],CameraColorVal[m_videoFormat][4]);

    ui->CameraDefaultMapVal->setText(QString(CameraTmp));

    char UIColorTmp[16];
    sprintf(UIColorTmp, "%d", UIColorDefaultVal[0]);
    ui->UIcolor_line1->setText(QString(UIColorTmp));
    sprintf(UIColorTmp, "%d", UIColorDefaultVal[1]);
    ui->UIcolor_line2->setText(QString(UIColorTmp));
    sprintf(UIColorTmp, "%d", UIColorDefaultVal[2]);
    ui->UIcolor_line3->setText(QString(UIColorTmp));
    sprintf(UIColorTmp, "%d", UIColorDefaultVal[3]);
    ui->UIcolor_line4->setText(QString(UIColorTmp));
    sprintf(UIColorTmp, "%d", UIColorDefaultVal[4]);
    ui->UIcolor_line5->setText(QString(UIColorTmp));

}

void MainWindow::on_commandLinkButton_clicked()
{

    for(int i = 0; i < 5; i++)
    {
        RS_S32 uiColorVal = QLine_UIcolor_list[i]->text().toInt();
        RS_S32 colorVal = translate(i, uiColorVal);
        //qDebug() << "colorVal:" << colorVal;
        QString TRcolor = QString("%1").arg(colorVal);
        QLine_TRcolor_list[i]->setText(TRcolor);
    }
}

RS_S32 MainWindow::translate(int colorType,RS_S32 uiColorVal)
{

    RS_S32 maxColorVal = 63;
    RS_S32 colorVal = 0;

    int chnvideofmt = ui->cameraFormatBox->currentIndex();

    if(ColorMode == 1)
        maxColorVal = 255;

    if(uiColorVal >= 0 && uiColorVal <= maxColorVal)
    {

        if(uiColorVal == UIColorDefaultVal[colorType])
        {
            colorVal = CameraColorVal[chnvideofmt][colorType];
        }
        else if(uiColorVal > UIColorDefaultVal[colorType])
        {
            colorVal = (int)((uiColorVal*(maxColorVal - CameraColorVal[chnvideofmt][colorType]) + maxColorVal*(CameraColorVal[chnvideofmt][colorType] - \
                UIColorDefaultVal[colorType]))/(maxColorVal - UIColorDefaultVal[colorType]));
        }
        else if(uiColorVal < UIColorDefaultVal[colorType])
        {
            colorVal = (int)((uiColorVal*CameraColorVal[chnvideofmt][colorType])/UIColorDefaultVal[colorType]);
        }

    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("uiColorVal is err!");
        msgBox.exec();
        return maxColorVal;
    }
    char tmp[256];
    sprintf(tmp, "############ colorType:%d %0x  uicolorval:%d colorval:%d\n",colorType,chnvideofmt,uiColorVal,colorVal);
    qDebug() << tmp;
    return colorVal;

}


void MainWindow::on_cameraFormatBox_currentIndexChanged(int index)
{
    doRefreshCameraDefaultMapVal();
}

void MainWindow::on_ADchipTypeBox_currentIndexChanged(int index)
{
    doRefreshCameraDefaultMapVal();
}
