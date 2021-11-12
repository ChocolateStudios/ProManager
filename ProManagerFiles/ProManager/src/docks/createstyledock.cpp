#include "createstyledock.h"
#include "customs/customstylescontextmenu.h"
#include "customs/custompopupbutton.h"

#include <QtWidgets>

CreateStyleDock::CreateStyleDock(QMainWindow* mainWin, CustomStylesContextMenu* stylesMenu)
    : BaseDock(tr("Create New Style"), mainWin), stylesContextMenu(stylesMenu)
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

    QVector<QString> colorNames = {
        "Automatic",
        "Black",
        "Blue",
        "Turquoise",
        "BrightGreen",
        "Pink",
        "Red",
        "Yellow",
        "White",
        "DarkBlue",
        "Teal",
        "Green",
        "Violet",
        "DarkRed",
        "DarkYellow",
        "Brown",
        "OliveGreen",
        "DarkGreen",
        "DarkTeal",
        "Indigo",
        "Orange",
        "BlueGray",
        "LightOrange",
        "Lime",
        "SeaGreen",
        "Aqua",
        "LightBlue",
        "Gold",
        "SkyBlue",
        "Plum",
        "Rose",
        "Tan",
        "LightYellow",
        "LightGreen",
        "LightTurquoise",
        "PaleBlue",
        "Lavender",
        "Gray05",
        "Gray10",
        "Gray125",
        "Gray15",
        "Gray20",
        "Gray25",
        "Gray30",
        "Gray35",
        "Gray375",
        "Gray40",
        "Gray45",
        "Gray50",
        "Gray55",
        "Gray60",
        "Gray625",
        "Gray65",
        "Gray70",
        "Gray75",
        "Gray80",
        "Gray85",
        "Gray875",
        "Gray90",
        "Gray95"
    };

    CustomPopupButton* FontColorBtn = new CustomPopupButton("", 100);

    QAction* autoAct = new QAction(colorNames[0]);
    connect(autoAct, &QAction::triggered, stylesContextMenu, &CustomStylesContextMenu::setFontColorForUpdateStyle);
    FontColorBtn->addActionToMenu(autoAct);
    emit autoAct->triggered();

    for (int i = 0; i < colorNames.size(); i++) {
        QAction* act = new QAction(colorNames[i]);
        connect(act, &QAction::triggered, stylesContextMenu, &CustomStylesContextMenu::setFontColorForNewStyle);
        FontColorBtn->addActionToMenu(act);
    }

    QHBoxLayout* headerOptions = new QHBoxLayout;
    QCheckBox* isHeaderCheckBox = new QCheckBox(tr("is Header"));
    headerLevelBtn = new CustomPopupButton("");
    headerLevelBtn->setVisible(false);
    connect(isHeaderCheckBox, &QCheckBox::toggled, this, [&](bool checked) {
        if (!checked)
            stylesContextMenu->setHeaderLevelForNewStyle();
        headerLevelBtn->setVisible(checked);
    });

    QAction* header1Act = new QAction(tr("Header 1"));
    connect(header1Act, &QAction::triggered, stylesContextMenu, &CustomStylesContextMenu::setHeaderLevelForNewStyle);
    headerLevelBtn->addActionToMenu(header1Act);

    for (int i = 0; i < 4; i++) {
        QAction* headerAct = new QAction(QString(tr("Header %1")).arg(i + 2));
        connect(headerAct, &QAction::triggered, stylesContextMenu, &CustomStylesContextMenu::setHeaderLevelForNewStyle);
        headerLevelBtn->addActionToMenu(headerAct);
    }

    QPushButton* createStyleBtn = new QPushButton("Create");

    connect(rectColorBtn, &QToolButton::clicked, stylesContextMenu, &CustomStylesContextMenu::setIconColorForNewStyle);
    connect(fontBtn, &QPushButton::clicked, stylesContextMenu, &CustomStylesContextMenu::setFontForNewStyle);
    connect(createStyleBtn, &QPushButton::clicked, stylesContextMenu, &CustomStylesContextMenu::addStyleFromDock);
    connect(createStyleBtn, &QPushButton::clicked, this, [&]() {
        autoDestroy();
    });

    QWidget* verticalSpacer = new QWidget;
    verticalSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addWidget(styleNameLabel);
    layout->addWidget(styleNameLEdit);

    rectColorLayout->addWidget(rectColorLabel);
    rectColorLayout->addWidget(rectColorBtn);
    layout->addLayout(rectColorLayout);

    fontPropsLayout->addWidget(fontLabel);
    fontOptionsLayout->addWidget(fontBtn);
    fontOptionsLayout->addWidget(FontColorBtn);
    fontPropsLayout->addLayout(fontOptionsLayout);
    layout->addLayout(fontPropsLayout);

    headerOptions->addWidget(isHeaderCheckBox);
    headerOptions->addWidget(headerLevelBtn);
    layout->addLayout(headerOptions);

    layout->addWidget(createStyleBtn);
    layout->addWidget(verticalSpacer);

    widget->setLayout(layout);

    setWidget(widget);

    mainWin->addDockWidget(Qt::RightDockWidgetArea, this);
}
