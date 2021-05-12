#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "imagescaler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QGraphicsScene;

class QGraphicsPixmapItem;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

  ~MainWindow() override;

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

  void on_actionAbout_triggered();

private:
  Ui::MainWindow *ui;
  ImageScaler scaler;
  QGraphicsScene *scene;

  void updateList();

  void render(const QPixmap &pixmap);

  void resizeGraphicsView();

  // QWidget interface
protected:
  void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
