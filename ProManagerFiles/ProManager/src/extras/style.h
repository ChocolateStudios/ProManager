#ifndef STYLE_H
#define STYLE_H

#include <QFont>
#include <QColor>
#include "word/document.h"

class QToolButton;

class Style
{

public:
    Style();

    const QColor &getIconColor() const;
    const QFont &getFont() const;
    const QString &getName() const;
    OfficeLib::Word::WdColor getFontColor() const;
    const unsigned short &getHeaderLevel() const;

    void setIconColor(const QColor& newIconColor);
    void setFont(const QFont& newFont);
    void setName(const QString& newName);
    void setFontColor(OfficeLib::Word::WdColor newFontColor);
    void setHeaderLevel(const unsigned short &newHeaderLevel);

private:
    QColor iconColor;
    QFont font;
    QString name;
    OfficeLib::Word::WdColor fontColor;
    unsigned short headerLevel = 0;
};

#endif // STYLE_H
