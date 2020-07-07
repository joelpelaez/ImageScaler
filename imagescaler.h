#ifndef IMAGESCALER_H
#define IMAGESCALER_H

#include <vector>
#include <QSize>
#include <QImage>

class ImageScaler
{
public:
    ImageScaler();
    bool load(const QString& fileName);
    bool loadFromClipboard();
    bool exportToDirectory(const QString& dirName);
    void addSize(const QSize& size);
    void removeSize(const QSize& size);
    void clearSizes();
    QImage image() const;
    std::vector<QSize> listExportSizes() const;
private:
    QImage currentImage;
    std::vector<QSize> exportSizes;
    QString originalFileName;
};

#endif // IMAGESCALER_H
