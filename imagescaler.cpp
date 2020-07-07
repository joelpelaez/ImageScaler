#include "imagescaler.h"
#include <algorithm>
#include <QDebug>
#include <QFileInfo>
#include <QClipboard>
#include <QApplication>

ImageScaler::ImageScaler()
{

}

bool ImageScaler::loadFromClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    QImage image = clipboard->image();

    if (image.isNull()) {
        return false;
    }

    currentImage = image;

    // TODO: Search a better name
    originalFileName = "ClipboardImage";

    return true;
}

bool ImageScaler::load(const QString &fileName)
{
    QImage originalImage(fileName);

    if (originalImage.isNull())
        return false;

    currentImage = originalImage;

    originalFileName = QFileInfo(fileName).fileName();

    return true;
}

bool ImageScaler::exportToDirectory(const QString &dirName)
{
    if (currentImage.isNull()) {
        return false;
    }

    QString fileName = originalFileName.mid(0, originalFileName.lastIndexOf("."));
    QString extName = originalFileName.mid(originalFileName.lastIndexOf("."));
    for (auto size : exportSizes)
    {
        QImage exportImage = currentImage.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QString exportFile = fileName + "_" + QString::number(size.width()) + "x" + QString::number(size.height()) + extName;
        exportImage.save(dirName + "/" + exportFile);
    }

    return true;
}

void ImageScaler::addSize(const QSize &size)
{
    if (std::find(exportSizes.begin(), exportSizes.end(), size) == exportSizes.end())
        exportSizes.push_back(size);
}

void ImageScaler::removeSize(const QSize &size)
{
    exportSizes.erase(std::remove(exportSizes.begin(), exportSizes.end(), size));
}

void ImageScaler::clearSizes()
{
    exportSizes.clear();
}

QImage ImageScaler::image() const
{
    return currentImage;
}

std::vector<QSize> ImageScaler::listExportSizes() const
{
    return exportSizes;
}
