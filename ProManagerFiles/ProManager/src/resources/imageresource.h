#ifndef IMAGERESOURCE_H
#define IMAGERESOURCE_H

#include "resources/resource.h"

#include <QDir>
#include <QPixmap>

class QPixmap;

class ImageResource : public Resource
{
    friend QDataStream &operator>>(QDataStream& in, ImageResource& resource);
    friend QDataStream &operator<<(QDataStream& out, ImageResource resource);
public:
    ImageResource()
        : Resource()
    {
    }

    ImageResource(const QString& newName, QPixmap* newPixmap, const QString& path)
        : Resource(newName), pixmap(newPixmap), path(path)
    {
    }

    inline QPixmap *getPixmap() const { return pixmap; }
    inline void setPixmap(QPixmap *newPixmap) { pixmap = newPixmap; }

    inline const QString &getPath() const { return path; }
    inline void setPath(const QString &newPath) { path = newPath; }

private:
    QPixmap* pixmap;
    QString path;
};

#endif // IMAGERESOURCE_H
