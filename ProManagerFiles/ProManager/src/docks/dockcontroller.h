#ifndef DOCKCONTROLLER_H
#define DOCKCONTROLLER_H

#include <QMainWindow>

#include "docks/createstyledock.h"
#include "docks/editstyledock.h"
#include "docks/imagesdock.h"
#include "docks/navegationdock.h"
#include "docks/filesdock.h"

#include "customs/customstylescontextmenu.h"

enum DockType {
    CreateStyleDck = 0,
    EditStyleDck,
    ImagesDck,
    NavegationDck,
    FilesDck
};

class DockController : public QObject
{
    Q_OBJECT
public:
    DockController() : QObject()
    {
    }

    void Init(QMainWindow *mainWindow, CustomStylesContextMenu* stylesContxtMenu,
              QList<CustomImage>* importedImgs, QList<CustomFile>* importedFls, QString& docTitle, CustomTextEdit* txtEdit)
    {
        mainWin = mainWindow;
        stylesContextMenu = stylesContxtMenu;
        importedImages = importedImgs;
        importedFiles = importedFls;
        documentTitle = docTitle;
        textEdit = txtEdit;
    }

    void showOrHide(DockType type) {
        if (type == CreateStyleDck) {
            if (!createStyleDock) {
                createStyleDock = show<CreateStyleDock, CustomStylesContextMenu*>(stylesContextMenu);
                connect(createStyleDock, &QDockWidget::destroyed, this, [&]() { createStyleDock = nullptr; });
            }
            else {
                hide<CreateStyleDock>(createStyleDock);
            }
        }
        else if (type == EditStyleDck) {
            if (!editStyleDock) {
                editStyleDock = show<EditStyleDock, CustomStylesContextMenu*, CustomTextEdit*>(stylesContextMenu, textEdit);
                connect(editStyleDock, &QDockWidget::destroyed, this, [&]() { editStyleDock = nullptr; });
            }
            else {
                hide<EditStyleDock>(editStyleDock);
            }
        }
        else if (type == ImagesDck) {
            if (!imagesDock) {
                imagesDock = show<ImagesDock, QList<CustomImage>*>(importedImages);
                connect(imagesDock, &QDockWidget::destroyed, this, [&]() { imagesDock = nullptr; });
            }
            else {
                hide<ImagesDock>(imagesDock);
            }
        }
        else if (type == NavegationDck) {
            if (!navegationDock) {
                navegationDock = show<NavegationDock, const QList<Style>*, const QString&, CustomTextEdit*>(&stylesContextMenu->getStyles(), documentTitle, textEdit);
                connect(navegationDock, &QDockWidget::destroyed, this, [&]() { navegationDock = nullptr; });
            }
            else {
                hide<NavegationDock>(navegationDock);
            }
        }
        else if (type == FilesDck) {
            if (!filesDock) {
                filesDock = show<FilesDock, QList<CustomFile>*>(importedFiles);
                connect(filesDock, &QDockWidget::destroyed, this, [&]() { filesDock = nullptr; });
            }
            else {
                hide<FilesDock>(filesDock);
            }
        }
    }

    bool showing(DockType type) {
        if (type == CreateStyleDck) {
            if (createStyleDock)
                return true;
        }
        else if (type == EditStyleDck) {
            if (editStyleDock)
                return true;
        }
        else if (type == ImagesDck) {
            if (imagesDock)
                return true;
        }
        else if (type == NavegationDck) {
            if (navegationDock)
                return true;
        }
        else if (type == FilesDck) {
            if (filesDock)
                return true;
        }
        return false;
    }

private:
    template<typename T, typename... R>
    T* show(R... args)
    {
        T* dock = new T(mainWin, args...);
        dock->setVisible(true);
        return dock;
    }

    template<typename T>
    void hide(T* dock)
    {
        dock->setVisible(false);
        dock->autoDestroy();
    }

    CreateStyleDock* createStyleDock = nullptr;
    EditStyleDock* editStyleDock = nullptr;
    ImagesDock* imagesDock = nullptr;
    NavegationDock* navegationDock = nullptr;
    FilesDock* filesDock = nullptr;

    QMainWindow *mainWin = nullptr;
    CustomStylesContextMenu* stylesContextMenu = nullptr;
    QList<CustomImage>* importedImages = nullptr;
    QList<CustomFile>* importedFiles = nullptr;
    QString documentTitle;
    CustomTextEdit* textEdit = nullptr;
};

#endif // DOCKCONTROLLER_H
