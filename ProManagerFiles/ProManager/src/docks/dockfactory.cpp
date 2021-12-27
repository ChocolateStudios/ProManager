#include "dockfactory.h"

#include "docks/views/stylecreatordock.h"
#include "docks/views/styleeditordock.h"
#include "docks/views/imageresourceimporterdock.h"
#include "docks/views/navegationdock.h"
#include "docks/views/fileresourceimporterdock.h"
#include "docks/views/fileexportpropertiesdock.h"

#include "resources/controllers/resourcescontroller.h"

#include <QList>

DockFactory::DockFactory(DockArguments args)
    : arguments(args)
{
    dockCreationFunctions.append(std::bind(&DockFactory::createStyleCreatorDock, this, std::placeholders::_1));
    dockCreationFunctions.append(std::bind(&DockFactory::createStyleEditorDock, this, std::placeholders::_1));
    dockCreationFunctions.append(std::bind(&DockFactory::createImageResourceImporterDock, this, std::placeholders::_1));
    dockCreationFunctions.append(std::bind(&DockFactory::createFileResourceImporterDock, this, std::placeholders::_1));
    dockCreationFunctions.append(std::bind(&DockFactory::createNavegationDock, this, std::placeholders::_1));
    dockCreationFunctions.append(std::bind(&DockFactory::createFileExportPropertiesDock, this, std::placeholders::_1));
}

BaseDock *DockFactory::createDockById(const unsigned int &id)
{
    return dockCreationFunctions[id](arguments);
}

void DockFactory::deleteDock(BaseDock *dock)
{
    delete dock;
}

BaseDock *DockFactory::createStyleCreatorDock(DockArguments &args)
{
    return new StyleCreatorDock(DockArguments(args.mainWin)
        .add(args.pointers[0]).add(args.pointers[3]));
}

BaseDock *DockFactory::createStyleEditorDock(DockArguments &args)
{
    return new StyleEditorDock(DockArguments(args.mainWin)
        .add(&((ResourcesController<StyleResource>*)args.pointers[0])->getResources())
        .add(args.pointers[0]).add(args.pointers[3]));
}

BaseDock *DockFactory::createFileResourceImporterDock(DockArguments &args)
{
    return new FileResourceImporterDock(DockArguments(args.mainWin).add(args.pointers[2]));
}

BaseDock *DockFactory::createImageResourceImporterDock(DockArguments &args)
{
    return new ImageResourceImporterDock(DockArguments(args.mainWin).add(args.pointers[1]));
}

BaseDock *DockFactory::createNavegationDock(DockArguments &args)
{
    return new NavegationDock(DockArguments(args.mainWin)
        .add(args.pointers[4])
            .add(&((ResourcesController<StyleResource>*)args.pointers[0])->getResources()));
}

BaseDock *DockFactory::createFileExportPropertiesDock(DockArguments &args)
{
    return new FileExportPropertiesDock(DockArguments(args.mainWin).add(args.pointers[5])
        .add(args.pointers[6]).add(&((ResourcesController<FileResource>*)args.pointers[2])->getResources()));
}
