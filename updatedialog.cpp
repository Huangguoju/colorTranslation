#include "updatedialog.h"
#include "ui_updatedialog.h"
#include <QNetworkRequest>
#include "QNetworkAccessManager"
#include "QNetworkReply"

UpdateDialog *UpdateDialog::m_pSelf = NULL;

UpdateDialog::UpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    m_pSelf = this;
    ui->setupUi(this);
    this->setFixedSize(554, 320);
    //让进度条循环显示
    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);

    ui->ContentLabel1->setVisible(false);
    ui->ContentBody->setVisible(false);
    ui->ContentLabel2->setVisible(false);

    ui->OKButton->setVisible(false);
    ui->CancelButton->setVisible(false);
    ui->scrollArea->setVisible(false);

    ui->scrollArea->setBackgroundRole(QPalette::Light);
    ui->ContentBody->setAlignment(Qt::AlignLeft | Qt::AlignTop);


    UpgradFileDetect();


    //QTimer::singleShot(300, this, SLOT(finish()));
}

UpdateDialog::~UpdateDialog()
{
    m_pSelf = NULL;
    delete ui;
}

void UpdateDialog::finish()
{
    qDebug() << "   asdgs  " ;
    ui->label->setVisible(false);
    ui->progressBar->setVisible(false);

    ui->ContentLabel1->setVisible(true);
    ui->ContentBody->setVisible(true);
    ui->ContentBody->setText("gasdg asdga ssad gasdghsda hsa hasdhsa hsadh sadhsd a hsdah ag sdahgsda hasdh sadhsad hsadh sda");
    ui->ContentLabel2->setVisible(true);

    ui->OKButton->setVisible(true);
    ui->CancelButton->setVisible(true);
    ui->scrollArea->setVisible(true);
}


void UpdateDialog::on_OKButton_clicked()
{

}

void UpdateDialog::on_CancelButton_clicked()
{
    m_pSelf->close();
}

void UpdateDialog::UpgradFileDetect()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("https://swann-device-agent-repository.s3.amazonaws.com/cctv/Swann-Cctv-Agent-DVR-5580-Prod-1.1.6-D31D-2CX.sqsh?AWSAccessKeyId=AKIAIIF2XX5MBOLEO56A&Expires=1559271764&Signature=LGPJCY9KcFLIN3VzF%2FaB7PpQdl8%3D")));
}

void UpdateDialog::replyFinished(QNetworkReply *reply){
    //获取响应的信息，状态码为200表示正常

    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QByteArray bytes = reply->readAll();  //获取字节
    QString result(bytes);  //转化为字符串
    qDebug()<<result;

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
//        download_Btn->setText("success");
//        QByteArray bytes = reply->readAll();  //获取字节
//        QString result(bytes);  //转化为字符串
        qDebug() << "  NoError   " ;
        qDebug()<<result;
    }
    else
    {
        qDebug() << "  failed   " ;
        //处理错误
        // download_Btn->setText("failed");
    }

    //reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;

}
