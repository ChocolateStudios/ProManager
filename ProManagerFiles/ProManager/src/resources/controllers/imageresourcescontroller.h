#ifndef IMAGERESOURCESCONTROLLER_H
#define IMAGERESOURCESCONTROLLER_H

#include "resources/controllers/resourcescontroller.h"
#include "resources/imageresource.h"

class ImageResourcesController : ResourcesController<ImageResource>
{
public:
    ImageResourcesController()
    {
    }

    ImageResource* getImageByName(const QString& imageName) {
        int size = resources.size();
        for(int i = 0; i < size; i++) {
            if (resources[i].getName() == imageName)
                return &resources[i];
        }
        return nullptr;
    }
};

#endif // IMAGERESOURCESCONTROLLER_H
