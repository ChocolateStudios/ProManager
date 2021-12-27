#ifndef FILERESOURCEIMPORTERDOCK_H
#define FILERESOURCEIMPORTERDOCK_H

#include "docks/basedock.h"

struct DockArguments;

class QGridLayout;
class FileResource;

template<typename T>
class ResourcesController;

class FileResourceImporterDock : public BaseDock
{
    Q_OBJECT
    friend class DockFactory;

public:
    FileResourceImporterDock(DockArguments args);

private:
    bool isUniqueFile(const QString& fileName);
    void addFileImageButton(const FileResource& currentFile);

private slots:
    void importFile();

private:
    ResourcesController<FileResource>* fileResourcesController;
    unsigned int row, column;

    QWidget* scrollWidget;
    QGridLayout* filesImagesLayout;
};

#endif // FILERESOURCEIMPORTERDOCK_H
