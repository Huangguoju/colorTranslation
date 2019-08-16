#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutusdialog.h"
#include "updatedialog.h"

MainWindow *MainWindow::m_pSelf = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	m_pSelf = this;
	
	setFixedSize(904, 541);
	
    init();
	ui->movieLabel->clear();
	
    //当前软件版本
    oldversion = 1.1;
	
	//移除升级后的辅助升级bat脚本
    QFile::remove(QCoreApplication::applicationDirPath() + "/upgrad.bat");
    QFile::remove(QCoreApplication::applicationDirPath() + "/updateAssistant.bat");

    //首次启动检测是否有新版本
    m_detect = new FtpManager;
    m_detect->get("/colorTranslation/upgradInfo.txt", QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
    connect(m_detect, SIGNAL(sigfinish(bool)), this, SLOT(finish(bool)));

    m_famousRemark = new FtpManager;
    m_famousRemark->get("/colorTranslation/famousRemarkOnline.txt", QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
    connect(m_famousRemark, SIGNAL(sigfinish(bool)), this, SLOT(famousRemarkfinish(bool)));

    createActions();
    createMenus();
    createStatusBar();
}

MainWindow::~MainWindow()
{
    if (movie){
        movie->stop();
        disconnect(movie, SIGNAL(frameChanged(int)), this, SLOT(movieStatus(int)));
        movie->~QMovie();//要销毁qmovie才能删除图片
        ui->movieLabel->clear();
        ui->movieLabel->hide();
        QFile::remove(QCoreApplication::applicationDirPath() + "/startDemo.gif");
    }

    QFile::remove(QCoreApplication::applicationDirPath() + "/upgrad.bat");
    QFile::remove(QCoreApplication::applicationDirPath() + "/updateAssistant.bat");
    QFile::remove(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
    QFile::remove(QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");

    delete ui;
}

void MainWindow::movieStatus(int frameNumber)
{
    if (frameNumber == frameCount - 1){
        movie->stop();
        disconnect(movie, SIGNAL(frameChanged(int)), this, SLOT(movieStatus(int)));
        movie->~QMovie();//要销毁qmovie才能删除图片
        movie = NULL;
        ui->movieLabel->clear();
        ui->movieLabel->hide();
        QFile::remove(QCoreApplication::applicationDirPath() + "/startDemo.gif");
    }
}


void MainWindow::startDemofinish(bool result)
{
    if(!result){
        QFile::remove(QCoreApplication::applicationDirPath() + "/startDemo.gif");
        return;
    }

    movie = new QMovie(QCoreApplication::applicationDirPath() + "/startDemo.gif");
    ui->movieLabel->setScaledContents(true);//拉伸
    ui->movieLabel->setGeometry(0, 0, 892, 491);//图片位置
    ui->movieLabel->setMovie(movie);
    frameCount = movie->frameCount();
    movie->start();
    connect(movie, SIGNAL(frameChanged(int)), this, SLOT(movieStatus(int)));
}

void MainWindow::finish(bool result)
{
    /*
    forceUpgrad:0 //自动检测 0:检测到新版本不提示升级  1:检测到新版本并提示升级 2：检测到新版本不提示直接强制升级 3：忽略升级请求
    version:1.1
    showStartDemo:true
    connectUsUrl:http://www.baidu.com
    ftpExeName:33
    downloadExeName:车间配置项掩码计算工具
    info:
    增加一些功能，改善一些BUG。
    车间配置项掩码计算工具
    //无限升级：upgradInfo.txt里面的版本大于更新后的版本，且forceUpgrad=2，无限循环升级。
     */

    if(!result){
        QFile::remove(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
        QFile::remove(QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
        return;
    }

    QFile f(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "Open failed.";
        QFile::remove(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
        return;
    }

    QTextStream txtInput(&f);
    txtInput.setCodec("utf-8");
    QString lineStr;
    float newVersion = 0.0;
    int forceUpgradFlag = 0;
    m_connectUsUrl = "";
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        if(lineStr.startsWith("forceUpgrad", Qt::CaseInsensitive)){
            forceUpgradFlag = lineStr.mid(strlen("forceUpgrad:")).toInt();;
        }

        if(lineStr.startsWith("version", Qt::CaseSensitive)){
            newVersion = lineStr.mid(strlen("version:")).toFloat();
        }

        if(lineStr.startsWith("connectUsUrl", Qt::CaseSensitive)){
            m_connectUsUrl = lineStr.mid(strlen("connectUsUrl:"));
        }

        if(lineStr.startsWith("showStartDemo", Qt::CaseSensitive)){
            if (lineStr.mid(strlen("showStartDemo:")).toInt())
            {
                m_startDemo = new FtpManager;
                m_startDemo->get("/colorTranslation/startDemo.gif", QCoreApplication::applicationDirPath() + "/startDemo.gif");
                connect(m_startDemo, SIGNAL(sigfinish(bool)), this, SLOT(startDemofinish(bool)));
            }
        }
    }
    f.close();
    //删除文件
    QFile::remove(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");

    //自动检测 0:检测到新版本不提示升级  1:检测到新版本并提示升级 2：检测到新版本不提示直接强制升级 3：忽略升级请求
    if(newVersion > oldversion && forceUpgradFlag != 3)
    {
        if(forceUpgradFlag == 1 || forceUpgradFlag == 2)
            update();
    }
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

    for(int i = 0; i < 5; i++)
    {
        QLine_UIcolor_list[i]->clear();
        QLine_TRcolor_list[i]->clear();
    }

    ui->UIDefaultDisplayVal->clear();
    ui->CameraDefaultMapVal->clear();

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("文件");
    fileMenu->addAction(openAct);
    fileMenu->addSeparator(); //分隔符
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu("帮助");
//    helpMenu->addAction(helpAct);
    helpMenu->addAction(updateAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutUsAct);
}

void MainWindow::createActions()
{

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file")); //底部提示
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    exitAct = new QAction(QIcon(":image/exit.ico"), "退出", this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip("Exit the application");
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

//    helpAct = new QAction(QIcon(":image/help.ico"), "帮助", this);
//    helpAct->setShortcuts(QKeySequence::HelpContents); //添加快捷键
//    connect(helpAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    updateAct = new QAction("检查更新...", this);
    connect(updateAct, SIGNAL(triggered()), this, SLOT(update()));

    aboutUsAct = new QAction(QIcon(":image/about.ico"), "关于", this);
    aboutUsAct->setShortcuts(QKeySequence::WhatsThis); //添加快捷键
    connect(aboutUsAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::open()
{
    on_openFile_clicked();
}

void MainWindow::about()
{
    AboutUsDialog *aboutUsdia = new AboutUsDialog(this);
    aboutUsdia->exec();
}

void MainWindow::update()
{
    UpdateDialog *updateDia = new UpdateDialog(this);
    //无标题
    //updateDia->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);
    updateDia->exec();
}


void MainWindow::createStatusBar()
{
    statusBar()->showMessage("已就绪");

    famousRemark();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateFamous()));
    timer->start(10000);
}

void MainWindow::updateFamous()
{
    statusBar()->showMessage(famousRemarkVector.at(rand() % famousRemarkVector.size()));
}


void MainWindow::famousRemarkfinish(bool result)
{
    if(!result){
        QFile::remove(QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
        return;
    }

    QFile f(QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "Open failed.";
        QFile::remove(QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
        return;
    }

    QTextStream txtInput(&f);
    txtInput.setCodec("utf-8");
    QString lineStr;
    famousRemarkVector.clear();
    timer->stop();
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        if(lineStr.startsWith("interval", Qt::CaseSensitive)){
            //1秒：1000
            timer->setInterval(lineStr.mid(strlen("interval:")).toInt());
        }else{
            famousRemarkVector << lineStr;
        }
    }
    f.close();
    //删除文件
    QFile::remove(QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
    timer->start();
}

void MainWindow::famousRemark()
{
    srand(time(NULL));
    famousRemarkVector  << "泰戈尔曾经说过:要是童年的日子能重新回来，那我一定不再浪费光阴，我要把每分每秒都用来读书。"
        << "三年二班的周杰伦,请马上到教导处来"
        << "真理惟一可靠的标准就是永远自相符合。"
        << "时间是一切财富中最宝贵的财富。"
        << "世界上一成不变的东西，只有“任何事物都是在不断变化的”这条真理。"
        << "生活有度，人生添寿。 —— 书摘"
        << "任何事物都无法抗拒吞食一切的时间。"
        << "青春是没有经验和任性的。"
        << "思想以自己的言语喂养它自己，而成长起来。"
        << "泰戈尔曾经说过：沉默是一种美德，但在喜欢的人面前沉默那便是懦弱，我已经懦弱了三年，"
        "今天我要勇敢。黄晶晶同学，你愿意和我携手告别高中时代吗！ - 青春派"
        << "过放荡不羁的生活，容易得像顺水推舟，但是要结识良朋益友，却难如登天。"
        << "这世界要是没有爱情，它在我们心中还会有什么意义！这就如一盏没有亮光的走马灯。"
        << "人生并不像火车要通过每个站似的经过每一个生活阶段。人生总是直向前行走，从不留下什么。"
        << "有所作为是生活的最高境界。 —— 恩格斯"
        << "我们活着不能与草木同腐，不能醉生梦死，枉度人生，要有所做为。"
        << "浪费别人的时间等于是谋财害命，浪费自己的时间等于是慢性自杀。 —— 列宁"
        << "现实是此岸，理想是彼岸，中间隔着湍急的河流，行动则是架在河上的桥梁。 —— 克雷洛夫"
        << "读书谓已多，抚事知不足。 —— 王安石"
        << "青年长于创造而短于思考，长于猛干而短于讨论，长于革新而短于持重。 —— 培根"
        << "理想对我来说，具有一种非凡的魅力。我的理想总是充满着生活和泥土气息。我从来都不去空想那些不可能实现的事情。 —— 奥斯特洛夫斯基"
        << "青春啊，难道你始终囚禁在狭小圈子里？你得撕破老年的蛊惑人心的网。";
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

void MainWindow::on_openFile_clicked(bool hideDialog)
{
    if(hideDialog != 1){
        filename = QFileDialog::getOpenFileName(this,tr("open custom-config.ini"),
                                                QDir::currentPath(), tr("configuration files (*.ini)"));

        if (!filename.isEmpty()) {
            if (ui->directoryComboBox->findText(filename) == -1)
                ui->directoryComboBox->addItem(filename);
            ui->directoryComboBox->setCurrentIndex(ui->directoryComboBox->findText(filename));
        }
    }else{
        filename = ui->directoryComboBox->currentText();
    }

    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return;


    QSettings config(filename, QSettings::IniFormat);
//    QTextStream in(&file);
//    while (!in.atEnd()) {
//        QString line = in.readLine();
//        qDebug() << line ;
//    }
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
    UNUSED(index);
    index = 0;
    doRefreshCameraDefaultMapVal();
}

void MainWindow::on_ADchipTypeBox_currentIndexChanged(int index)
{
    UNUSED(index);
    index = 0;
    doRefreshCameraDefaultMapVal();
}

void MainWindow::on_directoryComboBox_currentIndexChanged(int index)
{
    UNUSED(index);
    index = 0;
    on_openFile_clicked(true);
}
