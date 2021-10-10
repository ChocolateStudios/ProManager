#include "createstyledock.h"
#include "customs/customstylescontextmenu.h"

#include <QtWidgets>
#include <QMainWindow>

CustomStylesContextMenu* CreateStyleDock::stylesContextMenu;
QMainWindow* CreateStyleDock::mainWin;
bool CreateStyleDock::showing = false;
CreateStyleDock* CreateStyleDock::instance;

CreateStyleDock::CreateStyleDock(CustomStylesContextMenu* stylesMenu, QMainWindow* mainWin)
    : QDockWidget(tr("Create New Style"), mainWin)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* widget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout;

    QLabel* styleNameLabel = new QLabel(tr("Style Name"));
    QLineEdit* styleNameLEdit = new QLineEdit;

    QHBoxLayout* rectColorLayout = new QHBoxLayout;
    QLabel* rectColorLabel = new QLabel(tr("Style Color"));
    QToolButton* rectColorBtn = new QToolButton();

    QVBoxLayout* fontPropsLayout = new QVBoxLayout;
    QLabel* fontLabel = new QLabel(tr("Font Properties"));
    QHBoxLayout* fontOptionsLayout = new QHBoxLayout;
    QPushButton* fontBtn = new QPushButton(tr("Font"));
    QToolButton* rectFontColorBtn = new QToolButton();

    QPushButton* createStyleBtn = new QPushButton("Create");

    connect(rectColorBtn, &QToolButton::clicked, stylesContextMenu, &CustomStylesContextMenu::setIconColorForNewStyle);
    connect(fontBtn, &QPushButton::clicked, stylesContextMenu, &CustomStylesContextMenu::setFontForNewStyle);
    connect(rectFontColorBtn, &QToolButton::clicked, stylesContextMenu, &CustomStylesContextMenu::setFontColorForNewStyle);
    connect(createStyleBtn, &QPushButton::clicked, stylesContextMenu, &CustomStylesContextMenu::addStyle);

    layout->addWidget(styleNameLabel);
    layout->addWidget(styleNameLEdit);

    rectColorLayout->addWidget(rectColorLabel);
    rectColorLayout->addWidget(rectColorBtn);
    layout->addLayout(rectColorLayout);

    layout->addStretch(1);

    fontPropsLayout->addWidget(fontLabel);
    fontOptionsLayout->addWidget(fontBtn);
    fontOptionsLayout->addWidget(rectFontColorBtn);
    fontPropsLayout->addLayout(fontOptionsLayout);
    layout->addLayout(fontPropsLayout);

    layout->addStretch(5);

    layout->addWidget(createStyleBtn);
    widget->setLayout(layout);

    setWidget(widget);
    setVisible(true);
    //stylesContextMenu->setAddStyleAction(toggleViewAction());
    mainWin->addDockWidget(Qt::RightDockWidgetArea, this);
    //viewMenu->addAction(toggleViewAction());
}

void CreateStyleDock::Init(CustomStylesContextMenu *stylesMenu, QMainWindow *win)
{
    stylesContextMenu = stylesMenu;
    mainWin = win;
}

void CreateStyleDock::showAndHide()
{
    if (!showing) {
        if (!instance)
            instance = new CreateStyleDock(stylesContextMenu, mainWin);
        instance->setVisible(true);
        showing = true;
    }
    else {
        if (instance) {
            instance->setVisible(false);
            delete instance;
            instance = nullptr;
        }
        showing = false;
    }
}

void CreateStyleDock::closeEvent(QCloseEvent *event)
{
    CreateStyleDock::showAndHide();
}
