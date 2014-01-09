#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include "qbinarydatasource.h"

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

public slots:
    void on_action_gotoAddress_triggered();
    void on_action_commitChanges_triggered();
    void on_action_revertChanges_triggered();
    void on_action_Open_triggered();
    void on_action_Quit_triggered();

signals:

private:
    Ui::MainWindow *ui;

    QFile *currentFile_;
    QBinaryDataSource *currentDS_;

};

#endif // MAINWINDOW_H
