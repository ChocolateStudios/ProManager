#ifndef CREATESTYLEDOCK_H
#define CREATESTYLEDOCK_H

#include "docks/basedock.h"

class CustomStylesContextMenu;
class CustomPopupButton;

class CreateStyleDock : public BaseDock
{
    Q_OBJECT
    friend class DockController;

private:
    CreateStyleDock(QMainWindow* mainWin, CustomStylesContextMenu* stylesMenu);
    CustomStylesContextMenu* stylesContextMenu;

    CustomPopupButton* headerLevelBtn;
};

#endif // CREATESTYLEDOCK_H
