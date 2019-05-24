#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QDragEnterEvent>
#include <QDebug>
#include <QGraphicsView>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include "QVector"
#include <QNetworkAccessManager>
#include "QTimer"
#include "time.h"

namespace Ui {
class MainWindow;
}

typedef unsigned char           RS_U8;
typedef unsigned short          RS_U16;
typedef unsigned int            RS_U32;
typedef unsigned long           RS_ULONG;
typedef unsigned long long      RS_U64;

typedef char                    RS_S8;
typedef short                   RS_S16;
typedef int                     RS_S32;
typedef long                    RS_SLONG;
typedef long long               RS_S64;

typedef enum
{
    COLOR_HUE_E = 0,
    COLOR_BRIGHTNESS_E = 1,
    COLOR_CONTRAST_E = 2,
    COLOR_SATURATION_E = 3,
    COLOR_SHARPNESS_E = 4
}COLOR_TYPE_E;

/* 前端AD 芯片的类型(AHD, TVI, ...)*/
typedef enum _RS_AD_CHIP_TYPE_E
{
    RS_AD_CHIP_TYPE_NORMAL = 1,
    RS_AD_CHIP_TYPE_AHD,
    RS_AD_CHIP_TYPE_TVI,
    RS_AD_CHIP_TYPE_AHD_TVI,
    RS_AD_CHIP_TYPE_PR1000,
    RS_AD_CHIP_TYPE_AHD_6134B,
    RS_AD_CHIP_TYPE_AHD_6134,
    RS_AD_CHIP_TYPE_AHD_6134C,
    RS_AD_CHIP_TYPE_TVI_2826,
    RS_AD_CHIP_TYPE_TVI_2827,
    RS_AD_CHIP_TYPE_TVI_2827C,
    RS_AD_CHIP_TYPE_AHD_6158,
    RS_AD_CHIP_TYPE_AHD_6158C,
    RS_AD_CHIP_TYPE_TVI_2829,
    RS_AD_CHIP_TYPE_TVI_2828,
    RS_AD_CHIP_TYPE_UNKNOW
}RS_AD_CHIP_TYPE_E;

typedef enum
{
    VIDEO_NORMAL = 0,
    VIDEO_720P,
    VIDEO_720P_T,
    VIDEO_720P_C,
    VIDEO_1080P,
    VIDEO_1080P_T,
    VIDEO_1080P_C,
    VIDEO_3M_A,
    VIDEO_3M_T,
    VIDEO_3M_C,
    VIDEO_QXGA_A,
    VIDEO_QXGA_T,
    VIDEO_4M_A,
    VIDEO_4M_T,
    VIDEO_4M_C,
    VIDEO_5M_A,
    VIDEO_5M_T,
    VIDEO_8M_A,
    VIDEO_8M_T,
    VIDEO_8M_C,
    VIDEO_MAX
}AHDTVI_VIDEO_FORMAT_E;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    RS_S32 translate(int colorType, RS_S32 uiColorVal);
    int ReadColorSetting(const QSettings &config, const QString &key, RS_U8 *pColorSettting);
    void ConfigCameraColor(int chiptype);

private slots:
    void init();
    void on_commandLinkButton_clicked();
    void doRefreshCameraDefaultMapVal();

    void on_openFile_clicked(bool hideDialog = 0);
    void on_cameraFormatBox_currentIndexChanged(int index);
    void on_ADchipTypeBox_currentIndexChanged(int index);

    void createActions();
    void createMenus();
    void createStatusBar();
    void famousRemark();
    void updateFamous();

    void open();
    void about();
    void update();

    void on_directoryComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QList<QLineEdit *> QLine_UIcolor_list;
    QList<QLineEdit *> QLine_TRcolor_list;
    QVector<QString> famousRemarkVector;


    RS_U8 CameraColorVal[VIDEO_MAX][5];
    RS_U8 UIColorDefaultVal[5];
    RS_U8 ColorMode;
    QString filename;

    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *openAct;
    QAction *exitAct;

    QAction *helpAct;
    QAction *updateAct;
    QAction *aboutUsAct;
};

#endif // MAINWINDOW_H
