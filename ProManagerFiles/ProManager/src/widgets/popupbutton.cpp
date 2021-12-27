#include "popupbutton.h"

#include <QMenu>

PopupButton::PopupButton()
{
    setPopupMode(QToolButton::InstantPopup);

    menu = new QMenu;
    setMenu(menu);
}

PopupButton::PopupButton(const QString &text, int width, int height)
{
    setText(text);
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

void PopupButton::addActionToMenu(QAction *action)
{
    if (defaultConnection) {
        connect(action, &QAction::triggered, this, [&]() {
            if (QAction* a = qobject_cast<QAction*>(sender())) {
                setText(a->text());
            }
        });
    }
    menu->addAction(action);
}

void PopupButton::setDefaultConnection(const bool &defaultConnection)
{
    this->defaultConnection = defaultConnection;
}

void PopupButton::clearActions()
{
    menu->clear();
}

const QList<QAction *> PopupButton::getMenuActions()
{
    return menu->actions();
}
