#include "navegationconfigdock.h"

#include "docks/dockarguments.h"

#include "resources/styleresource.h"

#include <QLabel>
#include <QListView>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QVBoxLayout>

NavegationConfigDock::NavegationConfigDock(DockArguments args)
    : BaseDock(args.mainWin, tr("Edit")), StyleResourcesObserver(args.get<QList<StyleResource>>(0))
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Widgets

    QWidget* widget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QLabel* orderLabel = new QLabel(tr("Style hierarchy"));
    mainLayout->addWidget(orderLabel);

    QListView* stylesListView = new QListView;
    model = new QStandardItemModel(0, 1, this);

    int size = styles->size();
    for (int i = 1; i < size; i++) {
        model->insertRow(i-1);

        QPixmap pixmap(100,100);
        pixmap.fill(styles->at(i).getIconColor());
        model->setData(model->index(i-1, 0), styles->at(i).getName(), Qt::ItemDataRole::DisplayRole);
        model->item(i-1, 0)->setIcon(QIcon(pixmap));
        model->item(i-1, 0)->setFlags(model->item(i-1, 0)->flags() ^ Qt::ItemIsDropEnabled ^ Qt::ItemIsEditable);
    }

    stylesListView->setModel(model);
    stylesListView->setMovement(QListView::Snap);
    stylesListView->setDragDropMode(QListView::InternalMove);
    stylesListView->setDefaultDropAction(Qt::DropAction::TargetMoveAction);

    // Layout

    mainLayout->addWidget(stylesListView);

    widget->setLayout(mainLayout);
    setWidget(widget);
}

void NavegationConfigDock::update()
{
    qDebug() << "must update";
}

const QStandardItemModel* NavegationConfigDock::getStyleHierarchy() const
{
    return model;
}
