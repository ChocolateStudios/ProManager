#include "resources/styleresource.h"

#include <QDataStream>

QDataStream &operator>>(QDataStream& in, StyleResource& resource) {
    QString name;
    QColor iconColor;
    QFont font;
    OfficeLib::Word::WdColor fontColor;
    unsigned short headerLevel = 0;

    in >> name >> iconColor >> font >> fontColor >> headerLevel;
    resource.setName(name);
    resource.setIconColor(iconColor);
    resource.setFont(font);
    resource.setFontColor(fontColor);
    resource.setHeaderLevel(headerLevel);
    return in;
}

QDataStream &operator<<(QDataStream& out, StyleResource resource) {
    out << resource.getName() << resource.getIconColor() << resource.getFont()
        << resource.getFontColor() << resource.getHeaderLevel();
    return out;
}
