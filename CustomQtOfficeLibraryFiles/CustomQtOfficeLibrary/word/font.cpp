#include "font.h"

#include <QVariant>
#include <QAxObject>

namespace OfficeLib { namespace Word {

Font::Font(QAxObject *font)
    : fontObj(font)
{
}

void Font::setFontSize(const unsigned short &size)
{
    if (!fontObj) return;
    fontObj->setProperty("Size", size);
}

void Font::setFontColor(WdColor &color)
{
    if (!fontObj) return;
    fontObj->dynamicCall("SetColor(WdColor)", color);
    fontObj->setProperty("Color", color);
}

QVariant Font::getFontColor()
{
    if (!fontObj) return QVariant();
    return fontObj->dynamicCall("Color()");
}

void Font::setFontName(const QString &font)
{
    if (!fontObj) return;
    fontObj->setProperty("Name", font);
}

void Font::setFontBold(const bool &value)
{
    if (!fontObj) return;
    fontObj->setProperty("Bold", value);
}

}}
