#ifndef IMAGERESOURCEIMPORTER_H
#define IMAGERESOURCEIMPORTER_H

#include "docks/basedock.h"

struct DockArguments;

class QGridLayout;
class ImageResource;

template<typename T>
class ResourcesController;

class ImageResourceImporterDock : public BaseDock
{
    Q_OBJECT
    friend class DockFactory;

public:
    ImageResourceImporterDock(DockArguments args);

private:
    bool isUniqueImage(const QString& imageName);
    void addImageButton(const ImageResource& currentImage);

private slots:
    void importImage();

private:
    ResourcesController<ImageResource>* imageResourcesController;
    unsigned int row, column;

    QGridLayout* imagesLayout;
    QWidget* scrollWidget;
};

#endif // IMAGERESOURCEIMPORTER_H
