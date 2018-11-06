#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

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

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    RS_S32 translate(int colorType, RS_S32 uiColorVal);
private slots:
    void init();
    void on_pushButton_clicked();

    void on_commandLinkButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<QLineEdit *> QLine_UIdefault_list;
    QList<QLineEdit *> QLine_UIcolor_list;
    QList<QLineEdit *> QLine_TRcolor_list;
    QList<QLineEdit *> QLine_Cameracolor_list;
};

#endif // MAINWINDOW_H
