#include "extras/style.h"

#include "customs/customcolorfontdialog.h"

#include <QToolButton>
#include <QFontDialog>
#include <QColorDialog>

Style::Style()
{
}

const QColor &Style::getIconColor() const
{
    return iconColor;
}

const QFont &Style::getFont() const
{
    return font;
}

const QString &Style::getName() const
{
    return name;
}

OfficeLib::Word::WdColor Style::getFontColor() const
{
    return fontColor;
}

void Style::setIconColor(const QColor& newIconColor)
{
    iconColor = newIconColor;
}

void Style::setFont(const QFont& newFont)
{
    font = newFont;
}

void Style::setName(const QString& newName)
{
    name = newName;
}

void Style::setFontColor(OfficeLib::Word::WdColor newFontColor)
{
    fontColor = newFontColor;
}

const unsigned short &Style::getHeaderLevel() const
{
    return headerLevel;
}

void Style::setHeaderLevel(const unsigned short &newHeaderLevel)
{
    headerLevel = newHeaderLevel;
}
