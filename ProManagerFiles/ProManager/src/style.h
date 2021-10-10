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

    void setIconColor(QToolButton* button);
    void setFont();
    void setName(const QString &newName);
    void setFontColor(QToolButton* button);

private:
    QColor iconColor;
    QFont font;
    QString name;
    OfficeLib::Word::WdColor fontColor;
};

#endif // STYLE_H
