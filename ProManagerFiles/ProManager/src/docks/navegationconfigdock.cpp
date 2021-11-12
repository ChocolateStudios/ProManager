#include "navegationconfigdock.h"

#include <QLabel>
#include <QListView>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QVBoxLayout>

NavegationConfigDock::NavegationConfigDock(QMainWindow *mWin, QDockWidget* navDck, const QList<Style> styles)
    : QDockWidget(tr("Edit"), mWin), mainWin(mWin), navDock(navDck)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* widget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QLabel* orderLabel = new QLabel(tr("Style hierarchy"));
    mainLayout->addWidget(orderLabel);

    QListView* stylesListView = new QListView;
    model = new QStandardItemModel(0, 1, this);

    for (int i = 1; i < styles.size() - 1; i++) {
        model->insertRow(i-1);

        QPixmap pixmap(100,100);
        pixmap.fill(styles[i].getIconColor());
        model->setData(model->index(i-1, 0), styles[i].getName(), Qt::ItemDataRole::DisplayRole);
        model->item(i-1, 0)->setIcon(QIcon(pixmap));
        model->item(i-1, 0)->setFlags(model->item(i-1, 0)->flags() ^ Qt::ItemIsDropEnabled ^ Qt::ItemIsEditable);
    }

    stylesListView->setModel(model);
    stylesListView->setMovement(QListView::Snap);
    stylesListView->setDragDropMode(QListView::InternalMove);
    stylesListView->setDefaultDropAction(Qt::DropAction::TargetMoveAction);

    mainLayout->addWidget(stylesListView);

    widget->setLayout(mainLayout);
    setWidget(widget);

    mainWin->addDockWidget(Qt::LeftDockWidgetArea, this);
}

const QStandardItemModel *NavegationConfigDock::getModel() const
{
    return model;
}

void NavegationConfigDock::closeEvent(QCloseEvent *event)
{
    QDockWidget::closeEvent(event);
    if (navDock) {
        navDock->setVisible(false);
        delete navDock;
    }
}
