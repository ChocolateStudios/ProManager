#ifndef POPUPBUTTON_H
#define POPUPBUTTON_H

#include <QList>
#include <QToolButton>

class QMenu;
class QAction;

class PopupButton : public QToolButton
{
public:
    PopupButton();
    PopupButton(const QString& text, int width = -1, int height = -1);

    void addActionToMenu(QAction* action);
    void setDefaultConnection(const bool& defaultConnection = true);
    void clearActions();

    const QList<QAction*> getMenuActions();

private:
    QMenu* menu;
    bool defaultConnection = true;
};

#endif // POPUPBUTTON_H
