#include "navegationdock.h"

#include "docks/navegationconfigdock.h"

#include "customs/customtextedit.h"

#include <QTreeView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QToolButton>

NavegationDock::NavegationDock(QMainWindow *mainWin, const QList<Style>* stls, const QString& docTitle, CustomTextEdit* txtEdit)
    : BaseDock(tr("Navegation"), mainWin), navConfigDock(new NavegationConfigDock(mainWin, this, *stls)), documentTitle(docTitle), textEdit(txtEdit)
{
    connect(navConfigDock, &QDockWidget::destroyed, this, [&]() { navConfigDock = nullptr; });

    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* widget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QToolButton* reloadBtn = new QToolButton;
    reloadBtn->setIcon(QIcon(":/images/reload.png"));
    connect(reloadBtn, &QToolButton::clicked, this, &NavegationDock::update);
    mainLayout->addWidget(reloadBtn);

    treeView = new QTreeView;

    update();

    treeView->setModel(model);

    mainLayout->addWidget(treeView);
    widget->setLayout(mainLayout);
    setWidget(widget);

    mainWin->addDockWidget(Qt::LeftDockWidgetArea, this);
    mainWin->tabifyDockWidget(this, navConfigDock);
}

NavegationDock::~NavegationDock()
{
    if (navConfigDock) {
        navConfigDock->setVisible(false);
        delete navConfigDock;
    }
}

void NavegationDock::update()
{
    if (model) delete model;

    model = new QStandardItemModel(0, 1);
    const QStandardItemModel* otherModel = navConfigDock->getModel();

    model->setHeaderData(0, Qt::Horizontal, "documentName");

    if (otherModel->rowCount() <= 0) return;

    textEdit->setNavegationTexts(model, otherModel);

//    QStandardItem* firstLevel = new QStandardItem(otherModel->item(0)->icon(), otherModel->item(0)->text());
//    firstLevel->setIcon(otherModel->item(0)->icon());
//    model->appendRow(firstLevel);

//    QStandardItem* lastLevel = firstLevel;
//    for (int i = 1; i < otherModel->rowCount(); i++) {
//        QStandardItem* level = new QStandardItem(otherModel->item(i)->icon(), otherModel->item(i)->text());
//        level->setIcon(otherModel->item(i)->icon());
//        lastLevel->appendRow(level);
//        lastLevel = level;
//    }

    treeView->setModel(model);

//    QStandardItem* secondLevel = new QStandardItem(otherModel->item(1)->icon(), otherModel->item(1)->text());
//    model->item(0)->appendRow(secondLevel);

//    QStandardItem* thridLevel = new QStandardItem(otherModel->item(2)->icon(), otherModel->item(2)->text());
//    model->item(0)->child(0)->appendRow(thridLevel);

//    QStandardItem* fourthLevel = new QStandardItem(otherModel->item(3)->icon(), otherModel->item(3)->text());
//    model->item(0)->appendRow(secondLevel);

//    model->insertRow(1);
//    model->setData(model->index(1, 0), "World");
//    model->setData(model->index(1,0), "xd");
//    model->item(0,0)->appendRow(new QStandardItem("lol xd"));
}
