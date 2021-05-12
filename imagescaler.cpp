#include "imagescaler.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QFileInfo>
#include <QMimeData>
#include <QUrl>

#include <algorithm>

ImageScaler::ImageScaler() {}

bool ImageScaler::loadFromClipboard() {
  QClipboard *clipboard = QApplication::clipboard();

  const QMimeData *data = clipboard->mimeData();

  if (data->hasImage()) {
    QImage image = clipboard->image();

    if (image.isNull()) {
      return false;
    }

    currentImage = image;

    // TODO: Search a better name
    originalFileName = "ClipboardImage";

    return true;
  } else if (data->hasUrls()) {
    QList<QUrl> urls = data->urls();
    QUrl url = urls.first();

    QString fileName = url.toLocalFile();

    QImage image = QImage(fileName);
    if (image.isNull()) {
      return false;
    }

    currentImage = image;

    originalFileName = QFileInfo(fileName).fileName();

    return true;
  }

  return false;
}

bool ImageScaler::load(const QString &fileName) {
  QImage originalImage(fileName);

  if (originalImage.isNull())
    return false;

  currentImage = originalImage;

  originalFileName = QFileInfo(fileName).fileName();

  return true;
}

bool ImageScaler::exportToDirectory(const QString &dirName) {
  if (currentImage.isNull()) {
    return false;
  }

  QString fileName = originalFileName.mid(0, originalFileName.lastIndexOf("."));
  QString extName = originalFileName.mid(originalFileName.lastIndexOf("."));
  for (auto size : exportSizes) {
    QImage exportImage = currentImage.scaled(size, Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation);
    QString exportFile = fileName + "_" + QString::number(size.width()) + "x" +
                         QString::number(size.height()) + extName;
    exportImage.save(dirName + "/" + exportFile);
  }

  return true;
}

void ImageScaler::addSize(const QSize &size) {
  if (std::find(exportSizes.begin(), exportSizes.end(), size) ==
      exportSizes.end())
    exportSizes.push_back(size);
}

void ImageScaler::removeSize(const QSize &size) {
  exportSizes.erase(std::remove(exportSizes.begin(), exportSizes.end(), size));
}

void ImageScaler::clearSizes() { exportSizes.clear(); }

QImage ImageScaler::image() const { return currentImage; }

std::vector<QSize> ImageScaler::listExportSizes() const { return exportSizes; }
