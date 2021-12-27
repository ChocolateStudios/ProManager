#ifndef STYLEEDITORDOCK_H
#define STYLEEDITORDOCK_H

#include "docks/basedock.h"

#include "observers/styleresourcesobserver.h"

#include "resources/styleresource.h"

struct DockArguments;

class StyleResource;
class StylesContextMenu;
class StyleResourcesController;

class StyleEditorDock : public BaseDock , public StyleResourcesObserver
{
    Q_OBJECT
    friend class DockFactory;

public:
    StyleEditorDock(DockArguments args);

    void update() override;

private:
    int indexUpdated;
    StyleResource styleUpdated;
    StyleResourcesController* styleResourcesController;
    StylesContextMenu* stylesContextMenu;
};

#endif // STYLEEDITORDOCK_H
