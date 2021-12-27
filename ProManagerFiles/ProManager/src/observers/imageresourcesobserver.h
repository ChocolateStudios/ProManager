#ifndef IMAGERESOURCESOBSERVER_H
#define IMAGERESOURCESOBSERVER_H

#include "observers/observer.h"

#include <QList>

class ImageResource;

class ImageResourcesObserver : public Observer
{
public:
    ImageResourcesObserver(QList<ImageResource>* _images)
        : images(_images)
    {
    }

protected:
    QList<ImageResource>* images;
};

#endif // IMAGERESOURCESOBSERVER_H
