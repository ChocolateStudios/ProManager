#ifndef EDITSTYLEDOCK_H
#define EDITSTYLEDOCK_H

#include "docks/basedock.h"

class QLineEdit;
class QToolButton;
class CustomStylesContextMenu;
class QPushButton;
class CustomPopupButton;
class QCheckBox;
class CustomTextEdit;

class EditStyleDock : public BaseDock
{
    Q_OBJECT
    friend class DockController;

private:
    EditStyleDock(QMainWindow* mainWin, CustomStylesContextMenu* stylesMenu, CustomTextEdit* txtEdit);

    CustomStylesContextMenu* stylesContextMenu;
    CustomTextEdit* textEdit;

    QLineEdit* styleNameLEdit;
    QToolButton* rectColorBtn;
    QPushButton* fontBtn;
    QCheckBox* isHeaderCheckBox;
    CustomPopupButton* FontColorBtn;
    CustomPopupButton* headerLevelBtn;
    QPushButton* updateStyleBtn;
    QString styleUpdatingName;
};

#endif // EDITSTYLEDOCK_H
