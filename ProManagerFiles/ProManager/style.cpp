#include "style.h"

Style::Style()
{
}

const QColor &Style::getIconColor() const
{
    return iconColor;
}

void Style::setIconColor(const QColor &newIconColor)
{
    iconColor = newIconColor;
}

const QFont &Style::getFont() const
{
    return font;
}

void Style::setFont(const QFont &newFont)
{
    font = newFont;
}

const QString &Style::getName() const
{
    return name;
}

void Style::setName(const QString &newName)
{
    name = newName;
}
