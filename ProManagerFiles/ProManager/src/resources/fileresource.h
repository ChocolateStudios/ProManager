#ifndef FILERESOURCE_H
#define FILERESOURCE_H

#include "resources/resource.h"

class FileResource : public Resource
{
    friend QDataStream &operator>>(QDataStream& in, FileResource& resource);
    friend QDataStream &operator<<(QDataStream& out, FileResource resource);

public:
    FileResource()
        : Resource()
    {
    }

    FileResource(const QString& newName, const QString& newPath)
        : Resource(newName), path(newPath)
    {
    }

    inline const QString &getPath() const { return path; }
    inline void setPath(const QString &newPath) { path = newPath; }

private:
    QString path;
};

#endif // FILERESOURCE_H
