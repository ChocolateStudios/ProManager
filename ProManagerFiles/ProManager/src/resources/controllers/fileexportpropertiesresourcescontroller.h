#ifndef FILEEXPORTPROPERTIESRESOURCESCONTROLLER_H
#define FILEEXPORTPROPERTIESRESOURCESCONTROLLER_H

#include "resources/controllers/resourcescontroller.h"
#include "resources/fileexportpropertiesresource.h"

class FileExportPropertiesResourcesController : public ResourcesController<FileExportPropertiesResource>
{
public:
    FileExportPropertiesResourcesController() {

    }

    FileExportPropertiesResource* getFileExportPropertiesByNameAndExtension(const QString& fileName) {
        int size = resources.size();
        for (int i = 0; i < size; i++) {
            const FileExportPropertiesResource& res = resources[i];
            if (QString("%1%2").arg(res.getName(), res.getExtension()) == fileName)
                return &resources[i];
        }
        return nullptr;
    }
};

#endif // FILEEXPORTPROPERTIESRESOURCESCONTROLLER_H
