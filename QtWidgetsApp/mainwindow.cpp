#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogsettings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_commonperiod->setText("");
    ui->progressBar->setValue(0);

    connect(ui->pushButton_import, SIGNAL(clicked()), ui->progressBar, SLOT(setValue(int)));
    ui->progressBar->setMaximum(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_import_clicked()
{
    int v = ui->progressBar->value() + 10;
    ui->progressBar->setValue(v > ui->progressBar->maximum() ? ui->progressBar->maximum() : v);
}

void MainWindow::on_actionOpen_triggered()
{
    DialogSettings dlg(this);
    //dlg.setParent(this, Qt::Popup);
    //dlg.setModal(true);
    dlg.exec();

    /* modaless dialog sample
     if (!findDialog) {
         findDialog = new FindDialog(this);
         connect(findDialog, SIGNAL(findNext()), this, SLOT(findNext()));
     }

     findDialog->show();
     findDialog->raise();
     findDialog->activateWindow();
     * */
}
