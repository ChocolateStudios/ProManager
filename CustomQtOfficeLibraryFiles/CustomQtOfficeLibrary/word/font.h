#ifndef FONT_H
#define FONT_H

#include "wordenums.h"

class QVariant;
class QString;
class QAxObject;

namespace OfficeLib { namespace Word {

class Font
{
public:
    Font(QAxObject* font);

    void setFontSize(const unsigned short& size);
    void setFontColor(WdColor& color);
    QVariant getFontColor();
    void setFontName(const QString& font);
    void setFontBold(const bool& value);

private:
    QAxObject* fontObj;
};

}}

#endif // FONT_H
