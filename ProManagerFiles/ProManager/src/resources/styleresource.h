#ifndef STYLERESOURCE_H
#define STYLERESOURCE_H

#include "resources/resource.h"

#include "word/wordenums.h"

#include <QFont>
#include <QColor>

class StyleResource : public Resource
{
    friend QDataStream &operator>>(QDataStream& in, StyleResource& resource);
    friend QDataStream &operator<<(QDataStream& out, StyleResource resource);
public:
    StyleResource()
        : Resource()
    {
        font.setFamily("Calibri");
        font.setPointSize(11);
    }

    StyleResource(const QString& newName, const QColor& newIconColor)
        : Resource(newName), iconColor(newIconColor)
    {
        font.setFamily("Calibri");
        font.setPointSize(11);
    }

    StyleResource(const QString& newName, const QColor& newIconColor, const QFont& newFont, OfficeLib::Word::WdColor newFontColor, const unsigned short &newHeaderLevel = 0)
        : Resource(newName), iconColor(newIconColor), font(newFont), fontColor(newFontColor), headerLevel(newHeaderLevel)
    {
    }

    inline const QColor &getIconColor() const { return iconColor; }
    inline const QFont &getFont() const { return font; }
    inline OfficeLib::Word::WdColor getFontColor() const { return fontColor; }
    inline const unsigned short &getHeaderLevel() const { return headerLevel; }

    inline void setIconColor(const QColor& newIconColor) { iconColor = newIconColor; }
    inline void setFont(const QFont& newFont) { font = newFont; }
    inline void setFontColor(OfficeLib::Word::WdColor newFontColor) { fontColor = newFontColor; }
    inline void setHeaderLevel(const unsigned short &newHeaderLevel) { headerLevel = newHeaderLevel; }

private:
    QColor iconColor;
    QFont font;
    OfficeLib::Word::WdColor fontColor;
    unsigned short headerLevel = 0;
};

#endif // STYLERESOURCE_H
