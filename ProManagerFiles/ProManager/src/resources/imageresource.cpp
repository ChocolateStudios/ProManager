#include "resources/imageresource.h"

#include <QDataStream>

QDataStream &operator>>(QDataStream& in, ImageResource& resource) {
    QString name;
    QPixmap* pixmap = new QPixmap();
    QString path;

    in >> name >> *pixmap >> path;
    resource.setName(name);
    resource.setPixmap(pixmap);
    resource.setPath(path);
    return in;
}

QDataStream &operator<<(QDataStream& out, ImageResource resource) {
    out << resource.getName() << (*resource.getPixmap()) << resource.getPath();
    return out;
}
