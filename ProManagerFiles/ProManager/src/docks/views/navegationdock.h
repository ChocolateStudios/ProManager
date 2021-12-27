#ifndef NAVEGATIONDOCK_H
#define NAVEGATIONDOCK_H

#include "docks/basedock.h"

class NavegationConfigDock;
class QStandardItemModel;
class QTreeView;
class TextAnalizer;

struct DockArguments;

class NavegationDock : public BaseDock
{
    Q_OBJECT
    friend class DockFactory;

public:
    NavegationDock(DockArguments args);
    ~NavegationDock();

private slots:
    void updateTreeModel();

private:
    TextAnalizer* textAnalizer;
    NavegationConfigDock* navConfigDock;

    QStandardItemModel* model = nullptr;
    QTreeView* treeView = nullptr;
    bool deleteOther = true;
};

#endif // NAVEGATIONDOCK_H
