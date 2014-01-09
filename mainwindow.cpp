#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qbinarydatasource.h"
#include "qbinarydatainterpretation.h"

#include <QFile>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    QFile *f = new QFile("/home/D.Iakovliev/mail.gpg");
    //QFile *f = new QFile("/tftpboot/SatelliteLive-20130830.ts");
    if ( !f->open(QIODevice::ReadWrite) )
    {
        qDebug("Unable to open file");
    }

    QBinaryDataSource *ds = new QBinaryDataSource(this);
    QBinaryDataInterpretation *interptretation = new QBinaryDataInterpretation(this);

    ds->attachTo(f);
    ds->setInterpretation(interptretation);

    ui->binaryDataView->setDataSource(ds);

    //--------------------------------------------
    connect(ui->actionGoto_address, SIGNAL(triggered()), this, SLOT(on_action_gotoAddress_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_gotoAddress_triggered()
{
    bool ok;
    QString addressStr = QInputDialog::getText(this,
                                               tr("Please enter address"),
                                               tr("Address in HEX"),
                                               QLineEdit::Normal,
                                               QString(),
                                               &ok);
    if (ok && !addressStr.isEmpty())
    {
        ui->binaryDataView->gotoAddress(addressStr.toULongLong(0,16));
    }
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
