#include "navegationdock.h"

#include "docks/dockarguments.h"
#include "docks/views/navegationconfigdock.h"

#include "resources/styleresource.h"

#include "analizers/textanalizer.h"

#include <QTreeView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QToolButton>

NavegationDock::NavegationDock(DockArguments args)
    : BaseDock(args.mainWin, tr("Navegation")),
      textAnalizer(args.get<TextAnalizer>(0)),
      navConfigDock(new NavegationConfigDock(DockArguments(args.mainWin).add(args.pointers[1])))
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    connect(navConfigDock, &QTabWidget::destroyed, this, [&]() {
        navConfigDock = nullptr;
        if (deleteOther)
            delete this;
    });

    // Widgets

    QWidget* widget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QToolButton* reloadBtn = new QToolButton;
    reloadBtn->setIcon(QIcon(":/images/reload.png"));
    mainLayout->addWidget(reloadBtn);

    treeView = new QTreeView;

    updateTreeModel();

    // Layout

    mainLayout->addWidget(treeView);
    widget->setLayout(mainLayout);
    setWidget(widget);

    args.mainWin->addDockWidget(Qt::LeftDockWidgetArea, this);
    args.mainWin->addDockWidget(Qt::LeftDockWidgetArea, navConfigDock);
    args.mainWin->tabifyDockWidget(this, navConfigDock);

    // Connections

    connect(reloadBtn, &QToolButton::clicked, this, &NavegationDock::updateTreeModel);
}

NavegationDock::~NavegationDock()
{
    if (navConfigDock) {
        navConfigDock->setVisible(false);
        deleteOther = false;
        delete navConfigDock;
    }
}

void NavegationDock::updateTreeModel()
{
    if (model) {
        delete model;
        model = nullptr;
    }

    const QStandardItemModel* styleHierarchy = navConfigDock->getStyleHierarchy();

    if (styleHierarchy->rowCount() <= 0) return;

    model = textAnalizer->getNavegationModel(styleHierarchy);

    if (model)
        treeView->setModel(model);
}
