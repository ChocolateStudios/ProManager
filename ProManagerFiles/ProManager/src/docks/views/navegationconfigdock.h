#ifndef NAVEGATIONCONFIGDOCK_H
#define NAVEGATIONCONFIGDOCK_H

#include "docks/basedock.h"
#include "observers/styleresourcesobserver.h"

class StyleResource;
class QStandardItemModel;

struct DockArguments;

class NavegationConfigDock : public BaseDock, public StyleResourcesObserver
{
    Q_OBJECT
    friend class NavegationDock;
public:
    NavegationConfigDock(DockArguments args);

    void update() override;

private:
    const QStandardItemModel* getStyleHierarchy() const;

    QStandardItemModel*  model;
    BaseDock* parentDock;
};

#endif // NAVEGATIONCONFIGDOCK_H
