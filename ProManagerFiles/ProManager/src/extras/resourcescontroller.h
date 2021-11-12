#ifndef RESOURCESCONTROLLER_H
#define RESOURCESCONTROLLER_H

#include <QList>

#include "customs/customimage.h"
#include "customs/customfile.h"

class ResourcesController
{
public:
    ResourcesController();

    void saveData(QDataStream& out);
    void readData(QDataStream& in);

public:
    QList<CustomImage> importedImages;
    QList<CustomFile> importedFiles;
};

#endif // RESOURCESCONTROLLER_H
