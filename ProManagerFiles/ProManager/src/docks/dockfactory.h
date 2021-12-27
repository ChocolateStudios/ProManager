#ifndef DOCKFACTORY_H
#define DOCKFACTORY_H

#include "docks/dockarguments.h"

class BaseDock;

class DockFactory
{
public:
    DockFactory(DockArguments args);

    BaseDock* createDockById(const unsigned int& id);
    void deleteDock(BaseDock* dock);

private:
    BaseDock* createStyleCreatorDock(DockArguments& args);
    BaseDock* createStyleEditorDock(DockArguments& args);
    BaseDock* createFileResourceImporterDock(DockArguments& args);
    BaseDock* createImageResourceImporterDock(DockArguments& args);
    BaseDock* createNavegationDock(DockArguments& args);
    BaseDock* createFileExportPropertiesDock(DockArguments& args);

private:
    DockArguments arguments;
    QList<std::function<BaseDock*(DockArguments& args)>> dockCreationFunctions;
};

#endif // DOCKFACTORY_H
