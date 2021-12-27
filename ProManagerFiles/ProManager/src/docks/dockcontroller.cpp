#include "dockcontroller.h"

#include "mainwindow.h"
#include "maintabwidget.h"

#include "resources/manager/resourcesmanager.h"

#include "docks/dockfactory.h"
#include "docks/basedock.h"

#include <QList>
#include <QMenu>

DockController::DockController(MainWindow* mainWin)
    : dockTypes(6)
{
    dockFactory = new DockFactory(DockArguments(mainWin)
            .add(mainWin->resourcesManager->getResourcesController<StyleResource>(0))
            .add(mainWin->resourcesManager->getResourcesController<ImageResource>(1))
            .add(mainWin->resourcesManager->getResourcesController<FileResource>(2))
            .add(mainWin->mainTabWidget->stylesContextMenu)
            .add(mainWin->mainTabWidget->textAnalizer)
            .add(mainWin->resourcesManager->getResourcesController<FileExportPropertiesResource>(3))
            .add(&mainWin->mainTabWidget->tabNames));

    connect(this, &DockController::dockById, this, &DockController::createOrDestroyDockById);

    createActions();

    for (int i = 0; i < dockTypes; i++) {
        docks.append(nullptr);

        const QIcon icon = QIcon(QString(":/images/dock-%1").arg(i));
        QAction* toggleDock = toggleDockActions[i];
        toggleDock->setIcon(icon);
        connect(toggleDock, &QAction::triggered, this, [&]() {
            if (QAction* act = qobject_cast<QAction*>(sender())) {
                int index = toggleDockActions.indexOf(act);
                emit dockById(index);
            }
        });

        mainWin->viewMenu->addAction(toggleDock);
    }
}

bool DockController::isDockVisible(const unsigned int &id)
{
    return docks[id];
}

QAction *DockController::getAddStyleAction()
{
    return toggleDockActions[0];
}

void DockController::createActions()
{
    QAction* toggleStyleCreatorDockAct = new QAction(tr("Create New Style"), this);
    toggleStyleCreatorDockAct->setStatusTip(tr("Create a new format style"));

    QAction* toggleStyleEditorDockAct = new QAction(tr("Edit a Style"), this);
    toggleStyleEditorDockAct->setStatusTip(tr("Edit selected style"));

    QAction* toggleImageResourceImporterDockAct = new QAction(tr("Images"), this);
    toggleImageResourceImporterDockAct->setStatusTip(tr("Insert and visualize imported images"));

    QAction* toggleFileResourceImporterDockAct = new QAction(tr("Files"), this);
    toggleFileResourceImporterDockAct->setStatusTip(tr("Insert and visualize imported files"));

    QAction* toggleNavegationDockAct = new QAction(tr("Navegation"), this);
    toggleNavegationDockAct->setStatusTip(tr("Navegation guide for your document"));

    QAction* toggleFileExportPropertiesDockAct = new QAction(tr("File Export Properties"), this);
    toggleNavegationDockAct->setStatusTip(tr("Edit file export properties"));

    toggleDockActions.append(toggleStyleCreatorDockAct);
    toggleDockActions.append(toggleStyleEditorDockAct);
    toggleDockActions.append(toggleImageResourceImporterDockAct);
    toggleDockActions.append(toggleFileResourceImporterDockAct);
    toggleDockActions.append(toggleNavegationDockAct);
    toggleDockActions.append(toggleFileExportPropertiesDockAct);
}

void DockController::createOrDestroyDockById(int id)
{
    if (BaseDock* d = docks[id]) {
        dockFactory->deleteDock(docks[id]);
    }
    else {
        docks[id] = dockFactory->createDockById(id);
        connect(docks[id], &BaseDock::destroyed, this, [&](QObject* obj) {
            int index = docks.indexOf(obj);
            docks[index] = nullptr;
        });
    }
}
