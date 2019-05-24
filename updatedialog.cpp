#include "updatedialog.h"
#include "ui_updatedialog.h"

UpdateDialog *UpdateDialog::m_pSelf = NULL;

UpdateDialog::UpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    m_pSelf = this;
    ui->setupUi(this);
    this->setFixedSize(554, 320);
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


    QTimer::singleShot(3000, this, SLOT(finish()));
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

