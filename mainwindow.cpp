#include <QDebug>
#include <QFile>
#include <QInputDialog>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qbinarydatasource.h"
#include "qbinarydatainterpretation.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    //--------------------------------------------
    //connect(ui->actionGoto_address, SIGNAL(triggered()), this, SLOT(on_action_gotoAddress_triggered()));
    //connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(on_action_Open_triggered()));
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

void MainWindow::closeCurrentFile()
{
    disconnect(ui->actionGoto_address, SIGNAL(triggered()), this, SLOT(on_action_gotoAddress_triggered()));
    ui->actionGoto_address->setEnabled(false);

    if (ui->binaryDataView->dataSource() != 0)
    {
        QBinaryDataSource *ds = ui->binaryDataView->dataSource();
        ui->binaryDataView->setDataSource(0);
        if (ds) {
            QIODevice *f = ds->detachFrom();
            if(f) {
                f->close();
                delete f;
            }
            delete ds;
        }
    }
}

void MainWindow::openFile(const QString &fileName)
{
    QFile *f = new QFile(fileName);
    if ( !f->open(QIODevice::ReadWrite) )
    {
        qDebug() << "Unable to open file:" << fileName;
        f->close();
        delete f;
        f = 0;
    }
    if (f)
    {
        QBinaryDataSource *ds = new QBinaryDataSource(ui->binaryDataView);
        ds->attachTo(f);
        ui->binaryDataView->setDataSource(ds);

        connect(ui->actionGoto_address, SIGNAL(triggered()), this, SLOT(on_action_gotoAddress_triggered()));
        ui->actionGoto_address->setEnabled(true);
    }
}

void MainWindow::on_action_Open_triggered()
{
    closeCurrentFile();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Any file (*.*)"));
    if (!fileName.isEmpty())
    {
        openFile(fileName);
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
