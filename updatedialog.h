#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class UpdateDialog;
}

class UpdateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UpdateDialog(QWidget *parent = 0);
    ~UpdateDialog();
private slots:
    void finish();
    void on_OKButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::UpdateDialog *ui;
    static UpdateDialog *m_pSelf;
};

#endif // UPDATEDIALOG_H
