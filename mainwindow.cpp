#include <QDebug>
#include <QFile>
#include <QInputDialog>
#include <QFileDialog>
#include <QSettings>
#include <QApplication>
#include <QClipboard>

#include "binbear.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qbinarydatasource.h"
#include "qbinarydatasourceproxy.h"

#include "qbinarydataformatsfactory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentFile_(0)
    , currentDS_(0)
    , colorSchemas_()
    , currentColorScheme_(0)
    , activeSelection_()
{    
    ui->setupUi(this);
    setupStatusBar();
    initColorSchemeActions();
    initCopyAsActions();

    //--------------------------------------------
    //connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(on_action_Open_triggered()));

    connect(ui->actionAbout_Qt, SIGNAL(triggered()), QApplication::instance(), SLOT(aboutQt()));

    readSettings();

    QStringList args = QCoreApplication::arguments();
    if (args.size() > 1) {
        openFile(args.at(1));
    }
}

MainWindow::~MainWindow()
{
    currentColorScheme_ = 0;
    qDeleteAll(colorSchemas_);
    delete ui;
}

void MainWindow::setupStatusBar()
{
    statusLabel_ = new QLabel(ui->statusBar);
    addressLabel_ = new QLabel(ui->statusBar);
    //fileStatusLabel_ = new QLabel(ui->statusBar);

    addressLabel_->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    addressLabel_->setMinimumWidth(100);
    //fileStatusLabel_->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //fileStatusLabel_->setMinimumWidth(100);

    ui->statusBar->insertWidget(-1, statusLabel_, 1);
    ui->statusBar->insertWidget(-1, addressLabel_, 0);
    //ui->statusBar->insertWidget(-1, fileStatusLabel_, 0);
}

void MainWindow::on_Copy_As_ActionTriggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QVariant formatter_id = action->property("formatter_id");
        QAbstractBinaryDataFormatter *formatter =
                QBinaryDataFormatsFactory::createFormatter((QBinaryDataFormatsFactory::Formatters)formatter_id.toInt(),
                                                           ui->binaryDataView->dataSource());
        if (formatter)
        {
            QByteArray formattedData = formatter->format(activeSelection_.begin, activeSelection_.end);
            qDebug() << QString::fromAscii(formattedData);

            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(QString::fromAscii(formattedData));

            delete formatter;
        }
    }
}

void MainWindow::initCopyAsActions()
{
    QMap<QBinaryDataFormatsFactory::Formatters, QString> formats = QBinaryDataFormatsFactory::supportedFormats();
    foreach(QBinaryDataFormatsFactory::Formatters fmt, formats.keys())
    {
        QAction *action = ui->menuCopy_As->addAction(formats[fmt]);
        action->setEnabled(false);
        action->setProperty("formatter_id", fmt);

        connect(action, SIGNAL(triggered()), this, SLOT(on_Copy_As_ActionTriggered()));
    }
}

void MainWindow::controlCopyAsActions(bool enable)
{
    QList<QAction *> actions = ui->menuCopy_As->actions();
    foreach(QAction *action, actions)
    {
        action->setEnabled(enable);
    }
}

void MainWindow::initColorSchemeActions()
{
    currentColorScheme_ = 0;
    qDeleteAll(colorSchemas_);

    QDir dir(":/color_schemas");
    colorSchemas_ = QBinaryDataColorSchemasFactory::createColorSchemas(dir);
    int i = 0;
    foreach(QBinaryDataColorScheme *scheme, colorSchemas_)
    {
        QAction *action = ui->menuColor_scheme->addAction(scheme->name());
        action->setEnabled(true);
        action->setProperty("scheme_id", i++);

        connect(action, SIGNAL(triggered()), this, SLOT(on_Color_Scheme_ActionTriggered()));
    }
}

void MainWindow::on_Color_Scheme_ActionTriggered()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action) return;

    QList<QAction *> actions = ui->menuColor_scheme->actions();
    foreach(QAction *action, actions)
    {
        action->setCheckable(false);
        action->setChecked(false);
    }

    QVariant scheme_id = action->property("scheme_id");
    if (!scheme_id.isNull())
    {
        QAbstractBinaryDataSource *dataSource =
                qobject_cast<QAbstractBinaryDataSource*>(ui->binaryDataView->dataSource());

        QBinaryDataColorScheme *scheme = colorSchemas_.at(scheme_id.toInt());
        if (currentColorScheme_ != scheme)
        {
            if (dataSource) scheme->setDataSource(dataSource);
            action->setCheckable(true);
            action->setChecked(true);
            qDebug() << "Color scheme:" << scheme->name() << "is set";

            currentColorScheme_ = scheme;
        }
        else
        {
            if (dataSource) scheme->setDataSource(0);
            action->setCheckable(false);
            action->setChecked(false);
            qDebug() << "Color scheme:" << scheme->name() << "is unset";

            currentColorScheme_ = 0;
        }
    }
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

void MainWindow::on_action_commitChanges_triggered()
{
    QBinaryDataSourceProxy *editorProxy =
            qobject_cast<QBinaryDataSourceProxy*>(ui->binaryDataView->dataSource());
    if (editorProxy && editorProxy->doWeHaveChangesToCommit())
    {
        editorProxy->commitChanges();
    }
}

void MainWindow::on_action_revertChanges_triggered()
{
    QBinaryDataSourceProxy *editorProxy =
            qobject_cast<QBinaryDataSourceProxy*>(ui->binaryDataView->dataSource());
    if (editorProxy && editorProxy->doWeHaveChangesToCommit())
    {
        editorProxy->revertChanges();
    }
}

void MainWindow::closeCurrentFile()
{    
    disconnect(ui->actionGoto_address, SIGNAL(triggered()), this, SLOT(on_action_gotoAddress_triggered()));
    disconnect(ui->actionCommit_changes, SIGNAL(triggered()), this, SLOT(on_action_commitChanges_triggered()));
    disconnect(ui->actionRevert_changes, SIGNAL(triggered()), this, SLOT(on_action_revertChanges_triggered()));

    ui->actionGoto_address->setEnabled(false);
    ui->actionCommit_changes->setEnabled(false);
    ui->actionRevert_changes->setEnabled(false);

    controlCopyAsActions(false);
    setWindowTitle(APP_NAME);

    disconnect(ui->binaryDataView->viewport(), SIGNAL(Cursor_positionChanged(const QModelIndex &, const QModelIndex &)));
    disconnect(ui->binaryDataView->viewport(), SIGNAL(Cursor_selectionChanged(const QModelIndex &, const QModelIndex &)));
    disconnect(ui->binaryDataView->viewport(), SIGNAL(Cursor_selectionDone(const QModelIndex &, const QModelIndex &)));
    disconnect(ui->binaryDataView->viewport(), SIGNAL(Cursor_selectionCanceled()));

    if (ui->binaryDataView->dataSource() != 0)
    {
        ui->binaryDataView->setDataSource(0);
        QBinaryDataSourceProxy *proxy = qobject_cast<QBinaryDataSourceProxy*>(ui->binaryDataView->dataSource());
        if (proxy)
        {
            delete proxy;
        }

        currentDS_->detachFrom();
        currentFile_->close();
        delete currentFile_;
        currentFile_ = 0;
        delete currentDS_;
        currentDS_ = 0;

        statusLabel_->clear();
        addressLabel_->clear();
        //fileStatusLabel_->clear();
    }
}

void MainWindow::on_viewport_Cursor_positionChanged(const QModelIndex &prev, const QModelIndex &current)
{
    Q_UNUSED(prev);

    addressLabel_->setText(QString().sprintf("0x%08X", (int)currentDS_->indexToOffset(current)));
}

void MainWindow::on_viewport_Cursor_selectionDone(const QModelIndex &begin, const QModelIndex &end)
{
    quint64 b = currentDS_->indexToOffset(begin);
    quint64 e = currentDS_->indexToOffset(end);

    activeSelection_.begin  = begin;
    activeSelection_.end    = end;

    controlCopyAsActions(true);

    statusLabel_->setText(QString()
        .sprintf("0x%08X - 0x%08X (%ld)", (int)b, (int)e, e-b+1));
}

void MainWindow::on_viewport_Cursor_selectionCanceled()
{
    statusLabel_->setText(QString());

    activeSelection_.begin  = QModelIndex();
    activeSelection_.end    = QModelIndex();

    controlCopyAsActions(false);
}

void MainWindow::openFile(const QString &fileName)
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    currentFile_ = new QFile(fileName);
    if ( !currentFile_->open(QIODevice::ReadWrite) )
    {
        qDebug() << "Unable to open file:" << fileName;
        currentFile_->close();
        delete currentFile_;
        currentFile_ = 0;
    }
    if (currentFile_)
    {
        currentDS_ = new QBinaryDataSource(ui->binaryDataView);
        currentDS_->attachTo(currentFile_);

        connect(ui->binaryDataView->viewport(), SIGNAL(Cursor_positionChanged(const QModelIndex &, const QModelIndex &))
                , this, SLOT(on_viewport_Cursor_positionChanged(const QModelIndex &, const QModelIndex &)));
        connect(ui->binaryDataView->viewport(), SIGNAL(Cursor_selectionChanged(const QModelIndex &, const QModelIndex &))
                , this, SLOT(on_viewport_Cursor_selectionDone(const QModelIndex &, const QModelIndex &)));
        connect(ui->binaryDataView->viewport(), SIGNAL(Cursor_selectionDone(const QModelIndex &, const QModelIndex &))
                , this, SLOT(on_viewport_Cursor_selectionDone(const QModelIndex &, const QModelIndex &)));
        connect(ui->binaryDataView->viewport(), SIGNAL(Cursor_selectionCanceled())
                , this, SLOT(on_viewport_Cursor_selectionCanceled()));

        QBinaryDataSourceProxy *proxy = currentDS_->createProxy();
        if (currentColorScheme_)
        {
            currentColorScheme_->setDataSource(proxy);
        }
        ui->binaryDataView->setDataSource(proxy);

        connect(ui->actionGoto_address, SIGNAL(triggered()), this, SLOT(on_action_gotoAddress_triggered()));
        connect(ui->actionCommit_changes, SIGNAL(triggered()), this, SLOT(on_action_commitChanges_triggered()));
        connect(ui->actionRevert_changes, SIGNAL(triggered()), this, SLOT(on_action_revertChanges_triggered()));

        ui->actionGoto_address->setEnabled(true);
        ui->actionCommit_changes->setEnabled(true);
        ui->actionRevert_changes->setEnabled(true);

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
