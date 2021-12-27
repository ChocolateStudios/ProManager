#include "resources/fileresource.h"

#include <QDataStream>

QDataStream &operator>>(QDataStream& in, FileResource& resource) {
    QString name;
    QString path;

    in >> name >> path;
    resource.setName(name);
    resource.setPath(path);
    return in;
}

QDataStream &operator<<(QDataStream& out, FileResource resource) {
    out << resource.getName() << resource.getPath();
    return out;
}
