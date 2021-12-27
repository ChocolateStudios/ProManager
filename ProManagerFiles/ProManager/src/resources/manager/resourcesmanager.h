#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "resources/controllers/styleresourcescontroller.h"
#include "resources/controllers/fileexportpropertiesresourcescontroller.h"
#include "resources/fileresource.h"
#include "resources/imageresource.h"
#include "resources/fileexportpropertiesresource.h"

class ResourcesManager
{
public:
    ResourcesManager() {
        styleResourcesControllers.append(new StyleResourcesController());
        styleResourcesControllers.append(new ResourcesController<ImageResource>());
        styleResourcesControllers.append(new ResourcesController<FileResource>());
        styleResourcesControllers.append(new FileExportPropertiesResourcesController());
    }

    template<typename T>
    ResourcesController<T>* getResourcesController(const unsigned int& index) {
        return (ResourcesController<T>*)styleResourcesControllers[index];
    }

    //Save and Read Data
    void readResources(QDataStream& in, StylesContextMenu* menu) {
        int size = styleResourcesControllers.size();
        for (int i = 0; i < size; i++)
            styleResourcesControllers[i]->readResources(in, menu);
    }
    void saveResources(QDataStream& out) {
        int size = styleResourcesControllers.size();
        for (int i = 0; i < size; i++)
            styleResourcesControllers[i]->saveResources(out);
    }

private:
    QList<BaseResourcesController*> styleResourcesControllers;
};

#endif // RESOURCESMANAGER_H
