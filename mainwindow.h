#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

public slots:
    void on_action_gotoAddress_triggered();
    void on_action_Open_triggered();

signals:

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
