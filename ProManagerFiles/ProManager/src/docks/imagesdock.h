#ifndef IMAGESDOCK_H
#define IMAGESDOCK_H

#include "docks/basedock.h"

class QGridLayout;
class CustomImage;

class ImagesDock : public BaseDock
{
    Q_OBJECT
    friend class DockController;

private:
    ImagesDock(QMainWindow *mainWin, QList<CustomImage>* importedImgs);
    bool isUniqueImage(const QString& imageName);
    void addImageButton(const CustomImage& currentImage);

private slots:
    void importImage();

private:
    QWidget* scrollWidget;
    QGridLayout* imagesLayout;
    unsigned int row, column;
    QList<CustomImage>* importedImages;
};

#endif // IMAGESDOCK_H
