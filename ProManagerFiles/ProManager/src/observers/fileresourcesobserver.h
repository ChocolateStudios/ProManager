#ifndef FILERESOURCESOBSERVER_H
#define FILERESOURCESOBSERVER_H

#include "observers/observer.h"

#include <QList>

class FileResource;

class FileResourcesObserver : public Observer
{
public:
    FileResourcesObserver(QList<FileResource>* _files)
        : files(_files)
    {
    }

protected:
    QList<FileResource>* files;
};

#endif // FILERESOURCESOBSERVER_H
