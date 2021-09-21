#ifndef STYLE_H
#define STYLE_H

#include <QFont>
#include <QColor>
#include "word/document.h"

class Style
{
public:
    Style();

    const QColor &getIconColor() const;
    void setIconColor(const QColor &newIconColor);
    const QFont &getFont() const;
    void setFont(const QFont &newFont);
    const QString &getName() const;
    void setName(const QString &newName);

    OfficeLib::Word::WdColor fontColor;

private:
    QColor iconColor;
    QFont font;
    QString name;
};

#endif // STYLE_H
