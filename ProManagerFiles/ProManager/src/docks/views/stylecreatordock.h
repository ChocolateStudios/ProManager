#ifndef STYLECREATORDOCK_H
#define STYLECREATORDOCK_H

#include "docks/basedock.h"

#include "observers/styleresourcesobserver.h"

#include "resources/styleresource.h"

struct DockArguments;

class StyleResource;
class StylesContextMenu;

template<typename T>
class ResourcesController;

class StyleCreatorDock : public BaseDock
{
    Q_OBJECT
    friend class DockFactory;

public:
    StyleCreatorDock(DockArguments args);

private:
    StyleResource styleCreated;
    ResourcesController<StyleResource>* styleResourcesController;
    StylesContextMenu* stylesContextMenu;

};

#endif // STYLECREATORDOCK_H
