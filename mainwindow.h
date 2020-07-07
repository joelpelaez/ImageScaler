#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imagescaler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_to_triggered();

    void on_actionWindows_Standard_Icons_triggered();

    void on_actionApple_iOS_AppIcon_triggered();

    void on_buttonRemove_clicked();

    void on_buttonRemoveAll_clicked();

    void on_buttonAdd_clicked();

    void on_actionLoad_from_Clipboard_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_Qt_triggered();

private:
    Ui::MainWindow *ui;
    ImageScaler scaler;
    void updateList();
};
#endif // MAINWINDOW_H
