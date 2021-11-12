#ifndef CUSTOMPOPUPBUTTON_H
#define CUSTOMPOPUPBUTTON_H

#include <QToolButton>

class CustomPopupButton : public QToolButton
{
    Q_OBJECT
public:
    CustomPopupButton(QString defaultText, int width = -1, int height = -1);
    void addActionToMenu(QAction* action);
    const QList<QAction*> getMenuActions() const;

private:
    QMenu* menu;
};

#endif // CUSTOMPOPUPBUTTON_H
