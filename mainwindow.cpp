#include "mainwindow.h"

#include <QFileDialog>
#include <QGraphicsItem>
#include <QMessageBox>

#include <algorithm>
#include <tuple>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  scene = new QGraphicsScene;
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionOpen_triggered() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open Image"), QString(), tr("Image Files (*.png *.jpg *.bmp)"));

  if (fileName.isNull())
    return;

  if (!scaler.load(fileName)) {
    QMessageBox message(this);
    message.setWindowTitle(tr("Cannot open file"));
    message.setText(tr("We cannot process this file."));
    message.setStandardButtons(QMessageBox::Ok);
    message.setIcon(QMessageBox::Warning);
    message.exec();
    return;
  }

  QPixmap pixmap(fileName);

  if (pixmap.isNull()) {
    QMessageBox message(this);
    message.setWindowTitle(tr("Cannot show image"));
    message.setText(tr("Image is not a valid format"));
    message.setStandardButtons(QMessageBox::Ok);
    message.setIcon(QMessageBox::Warning);
    message.exec();
    return;
  }

  // ui->imageLabel->setPixmap(pixmap);
  render(pixmap);
}

void MainWindow::updateList() {
  auto scales = scaler.listExportSizes();
  ui->listWidget->clear();
  ui->listWidget->addItems(std::accumulate(
      scales.begin(), scales.end(), QStringList(), [](QStringList a, QSize b) {
        a.push_back(QString::number(b.width()) + "x" +
                    QString::number(b.height()));
        return a;
      }));
}

void MainWindow::on_actionSave_to_triggered() {
  QString dirName =
      QFileDialog::getExistingDirectory(this, tr("Select Directory"));

  if (dirName.isNull())
    return;

  if (!scaler.exportToDirectory(dirName)) {
    QMessageBox message(this);
    message.setWindowTitle(tr("Cannot export image"));
    message.setText(
        tr("Check if a image was loaded or output directory permissions."));
    message.setStandardButtons(QMessageBox::Ok);
    message.setIcon(QMessageBox::Warning);
    message.exec();
  }

  QMessageBox ok(this);
  ok.setWindowTitle(tr("Export complete"));
  ok.setText(tr("All files were created successfully"));
  ok.setIcon(QMessageBox::Information);
  ok.exec();
}

void MainWindow::on_actionWindows_Standard_Icons_triggered() {
  scaler.clearSizes();
  scaler.addSize(QSize(16, 16));
  scaler.addSize(QSize(32, 32));
  scaler.addSize(QSize(64, 64));
  scaler.addSize(QSize(128, 128));
  updateList();
}

void MainWindow::on_actionApple_iOS_AppIcon_triggered() {
  scaler.clearSizes();
  scaler.addSize(QSize(20, 20));
  scaler.addSize(QSize(29, 29));
  scaler.addSize(QSize(40, 40));
  scaler.addSize(QSize(58, 58));
  scaler.addSize(QSize(60, 60));
  scaler.addSize(QSize(72, 72));
  scaler.addSize(QSize(76, 76));
  scaler.addSize(QSize(80, 80));
  scaler.addSize(QSize(87, 87));
  scaler.addSize(QSize(120, 120));
  scaler.addSize(QSize(152, 152));
  scaler.addSize(QSize(167, 167));
  scaler.addSize(QSize(180, 180));
  updateList();
}

void MainWindow::on_buttonRemove_clicked() {
  auto selectedItems = ui->listWidget->selectedItems();

  if (selectedItems.size() > 0) {
    for (const auto &item : selectedItems) {
      QString text = item->text();
      int width = 0, height = 0;
      width = text.split("x")[0].toInt();
      height = text.split("x")[1].toInt();
      scaler.removeSize(QSize(width, height));
    }
    updateList();
  }
}

void MainWindow::on_buttonRemoveAll_clicked() {
  scaler.clearSizes();
  updateList();
}

void MainWindow::on_buttonAdd_clicked() {
  int width = 0, height = 0;
  bool ok = false;

  width = ui->lineEditWidth->text().toInt(&ok);

  if (!ok) {
    return;
  }

  height = ui->lineEditHeight->text().toInt(&ok);

  if (!ok) {
    return;
  }

  scaler.addSize(QSize(width, height));
  updateList();
}

void MainWindow::on_actionLoad_from_Clipboard_triggered() {
  if (!scaler.loadFromClipboard()) {
    QMessageBox message(this);
    message.setWindowTitle(tr("Cannot load data from clipboard"));
    message.setText(tr("The copied data is not an image."));
    message.setStandardButtons(QMessageBox::Ok);
    message.setIcon(QMessageBox::Warning);
    message.exec();
    return;
  }

  QPixmap pixmap = QPixmap::fromImage(scaler.image());

  if (pixmap.isNull()) {
    QMessageBox message(this);
    message.setWindowTitle(tr("Cannot show image"));
    message.setText(tr("Image is not a valid format"));
    message.setStandardButtons(QMessageBox::Ok);
    message.setIcon(QMessageBox::Warning);
    message.exec();
    return;
  }

  // ui->imageLabel->setPixmap(pixmap);
  render(pixmap);
}

void MainWindow::render(const QPixmap &pixmap) {
  scene->clear();

  scene->addPixmap(pixmap);
  scene->setSceneRect(0, 0, pixmap.width(), pixmap.height());
  ui->graphicsView->setScene(scene);

  if (pixmap.height() > ui->graphicsView->height() &&
      pixmap.width() > ui->graphicsView->width()) {
    ui->graphicsView->ensureVisible(scene->sceneRect());
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->fitInView(scene->itemsBoundingRect(),
                                Qt::KeepAspectRatio);
  }
}

void MainWindow::resizeGraphicsView() {
  QPixmap pixmap = QPixmap::fromImage(scaler.image());
  render(pixmap);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  Q_UNUSED(event);
  resizeGraphicsView();
}

void MainWindow::on_actionExit_triggered() { QApplication::exit(0); }

void MainWindow::on_actionAbout_Qt_triggered() { QApplication::aboutQt(); }

void MainWindow::on_actionAbout_triggered()
{
  QMessageBox message(this);
  message.setWindowTitle(tr("About..."));
  message.setText(tr("ImageScaler compiled at " __DATE__));
  message.setStandardButtons(QMessageBox::Ok);
  message.setIcon(QMessageBox::Information);
  message.exec();
}

