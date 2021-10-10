#include "style.h"
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

void Style::setIconColor(QToolButton* button)
{
    QColor newIconColor = QColorDialog::getColor();
    button->setStyleSheet("border: 1px solid black; background-color: " + newIconColor.name() + ";");
    iconColor = newIconColor;
}

void Style::setFont()
{
    bool ok = false;
    QFont newFont = QFontDialog::getFont(&ok);

    if (ok)
        font = newFont;
}

void Style::setName(const QString &newName)
{
    name = newName;
}

void Style::setFontColor(QToolButton* button)
{
    OfficeLib::Word::WdColor newFontColor = CustomColorFontDialog::getColor();
    button->setStyleSheet("border: 1px solid black; background-color: " + QColor(Qt::red).name() + ";");
    fontColor = newFontColor;
}
