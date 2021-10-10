#include "customs/customaction.h"

CustomAction::CustomAction(QObject *parent) : QAction(parent)
{

}

CustomAction::CustomAction(Style style, QObject *parent) : QAction(parent)
{
    QPixmap pixmap(100,100);
    pixmap.fill(style.getIconColor());

    setIcon(QIcon(pixmap));
    setText(style.getName());

    this->style = &style;
}

const Style &CustomAction::getStyle() const
{
    return *style;
}

void CustomAction::setStyle(Style newStyle)
{
    style = &newStyle;
}
