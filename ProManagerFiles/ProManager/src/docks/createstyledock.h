#ifndef CREATESTYLEDOCK_H
#define CREATESTYLEDOCK_H

#include <QDockWidget>

class CustomStylesContextMenu;

class CreateStyleDock : public QDockWidget
{
    Q_OBJECT
public:
    CreateStyleDock(const CreateStyleDock&) = delete;
    static void Init(CustomStylesContextMenu* stylesMenu, QMainWindow* win);
    static void showAndHide();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    CreateStyleDock(CustomStylesContextMenu* stylesMenu, QMainWindow* mainWin);

    static CustomStylesContextMenu* stylesContextMenu;
    static QMainWindow* mainWin;
    static bool showing;
    static CreateStyleDock* instance;
};

#endif // CREATESTYLEDOCK_H
