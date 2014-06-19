#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QLabel>

#include "qbinarydatasource.h"
#include "qbinarydatacolorschemasfactory.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void closeCurrentFile();
    void openFile(const QString &fileName);

    void readSettings();
    void closeEvent(QCloseEvent *event);

    void setupStatusBar();

    void initCopyAsActions();
    void controlCopyAsActions(bool enable);

    void initColorSchemeActions();

public slots:
    void on_action_gotoAddress_triggered();
    void on_action_commitChanges_triggered();
    void on_action_revertChanges_triggered();
    void on_action_Open_triggered();
    void on_action_Quit_triggered();

    void on_viewport_Cursor_positionChanged(const QModelIndex &prev, const QModelIndex &current);
    void on_viewport_Cursor_selectionDone(const QModelIndex &begin, const QModelIndex &end);
    void on_viewport_Cursor_selectionCanceled();

    void on_Copy_As_ActionTriggered();
    void on_Color_Scheme_ActionTriggered();

signals:

private:
    Ui::MainWindow *ui;

    QFile *currentFile_;
    QBinaryDataSource *currentDS_;
    QList<QBinaryDataColorScheme*> colorSchemas_;
    QBinaryDataColorScheme *currentColorScheme_;
    QBinaryDataColorScheme *lastExternalColorScheme_;

    // status bar
    QLabel *statusLabel_;
    QLabel *addressLabel_;
    //QLabel *fileStatusLabel_;

    struct {
        QModelIndex begin;
        QModelIndex end;
    } activeSelection_;

};

#endif // MAINWINDOW_H
