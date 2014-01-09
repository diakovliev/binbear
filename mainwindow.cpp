#include <QDebug>
#include <QFile>
#include <QInputDialog>
#include <QFileDialog>
#include <QSettings>
#include <QCoreApplication>

#include "binbear.h"
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

    readSettings();

    QStringList args = QCoreApplication::arguments();
    if (args.size() > 1) {
        openFile(args.at(1));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSettings()
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
    QMainWindow::closeEvent(event);
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
    setWindowTitle(APP_NAME);

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
    QSettings settings(COMPANY_NAME, APP_NAME);

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

        settings.setValue("MainWindow/lastFile", fileName);
        setWindowTitle(QString("%1 <%2>").arg(APP_NAME).arg(QFileInfo(fileName).fileName()));
    }
}

void MainWindow::on_action_Open_triggered()
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    closeCurrentFile();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    settings.value("MainWindow/lastFile", "/home").toString(),
                                                    tr("Any file (*.*)"));
    if (!fileName.isEmpty())
    {
        openFile(fileName);
    }
}

void MainWindow::on_action_Quit_triggered()
{
    closeCurrentFile();

    close();
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
