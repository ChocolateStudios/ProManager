#include "custompopupbutton.h"

#include <QMenu>

CustomPopupButton::CustomPopupButton(QString defaultText, int width, int height)
{
    setText(defaultText);
    setPopupMode(QToolButton::InstantPopup);

    menu = new QMenu;
    setMenu(menu);

    if (width != -1) {
        setMinimumWidth(width);
        setMaximumWidth(width);
    }

    if (height != -1) {
        setMinimumHeight(height);
        setMaximumHeight(height);
    }
}

void CustomPopupButton::addActionToMenu(QAction *action)
{
    connect(action, &QAction::triggered, this, [&]() {
        if (QAction* a = qobject_cast<QAction*>(sender())) {
            setText(a->text());
        }
    });
    menu->addAction(action);
}

const QList<QAction *> CustomPopupButton::getMenuActions() const
{
    return menu->actions();
}
