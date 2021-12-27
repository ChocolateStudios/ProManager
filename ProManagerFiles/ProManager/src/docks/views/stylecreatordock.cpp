#include "stylecreatordock.h"

#include "word/wordcolorfontslist.h"

#include "docks/dockarguments.h"

#include "widgets/popupbutton.h"

#include "resources/controllers/resourcescontroller.h"

#include "menus/stylescontextmenu.h"

#include <QAction>
#include <QCheckBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

StyleCreatorDock::StyleCreatorDock(DockArguments args)
    : BaseDock(args.mainWin, tr("Create New Style")),
      styleResourcesController(args.get<ResourcesController<StyleResource>>(0)),
      stylesContextMenu(args.get<StylesContextMenu>(1))
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Widgets

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
    PopupButton* FontColorBtn = new PopupButton("Select a color", 100);

    QHBoxLayout* headerOptions = new QHBoxLayout;
    QCheckBox* isHeaderCheckBox = new QCheckBox(tr("is Header"));
    PopupButton* headerLevelBtn = new PopupButton("");
    headerLevelBtn->setVisible(false);

    QPushButton* createStyleBtn = new QPushButton("Create");

    QWidget* verticalSpacer = new QWidget;
    verticalSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Layout

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

    args.mainWin->addDockWidget(Qt::RightDockWidgetArea, this);



    // Connections

    QAction* autoAct = new QAction(OfficeLib::Word::colorNames[0]);
    connect(autoAct, &QAction::triggered, this, [&]() {
        QAction* act = qobject_cast<QAction*>(sender());
        OfficeLib::Word::WdColor newFontColor = OfficeLib::Word::colorFonts[OfficeLib::Word::colorNames.indexOf(act->text())];
        styleCreated.setFontColor(newFontColor);
    });
    FontColorBtn->addActionToMenu(autoAct);
    emit autoAct->triggered();

    int size = OfficeLib::Word::colorNames.size();
    for (int i = 1; i < size; i++) {
        QAction* act = new QAction(OfficeLib::Word::colorNames[i]);
        connect(act, &QAction::triggered, this, [&]() {
            QAction* act = qobject_cast<QAction*>(sender());
            OfficeLib::Word::WdColor newFontColor = OfficeLib::Word::colorFonts[OfficeLib::Word::colorNames.indexOf(act->text())];
            styleCreated.setFontColor(newFontColor);
        });
        FontColorBtn->addActionToMenu(act);
    }


    connect(isHeaderCheckBox, &QCheckBox::toggled, this, [&](bool checked) {
        if (!checked)
            styleCreated.setHeaderLevel(0);
        ((PopupButton*)this->widget()->children()[9])->setVisible(checked);
    });


    QAction* header1Act = new QAction(tr("Header 1"));
    connect(header1Act, &QAction::triggered, this, [&]() {
        QAction* act = qobject_cast<QAction*>(sender());
        bool ok = false;
        unsigned short newHeaderLevel = QString(act->text().at(act->text().size() - 1)).toShort(&ok);
        if (ok)
            styleCreated.setHeaderLevel(newHeaderLevel);
    });
    headerLevelBtn->addActionToMenu(header1Act);

    for (int i = 2; i < 6; i++) {
        QAction* headerAct = new QAction(QString(tr("Header %1")).arg(i));
        connect(headerAct, &QAction::triggered, this, [&]() {
            QAction* act = qobject_cast<QAction*>(sender());
            bool ok = false;
            unsigned short newHeaderLevel = QString(act->text().at(act->text().size() - 1)).toShort(&ok);
            if (ok)
                styleCreated.setHeaderLevel(newHeaderLevel);
        });
        headerLevelBtn->addActionToMenu(headerAct);
    }


    connect(rectColorBtn, &QToolButton::clicked, this, [&]() {
        QToolButton* btn = qobject_cast<QToolButton*>(sender());
        QColor newIconColor = QColorDialog::getColor();
        btn->setStyleSheet("border: 1px solid black; background-color: " + newIconColor.name() + ";");
        styleCreated.setIconColor(newIconColor);
    });

    connect(fontBtn, &QPushButton::clicked, this, [&]() {
        QPushButton* btn = qobject_cast<QPushButton*>(sender());
        bool ok = false;
        QFont newFont = QFontDialog::getFont(&ok);
        if (ok) {
            styleCreated.setFont(newFont);
            QFont btnFont = btn->font();
            btnFont.setBold(true);
            btn->setFont(btnFont);
        }
    });

    connect(createStyleBtn, &QPushButton::clicked, this, [&]() {
        styleCreated.setName(((QLineEdit*)this->widget()->children()[2])->text());

        if (styleCreated.getName().isEmpty())
            return;

        styleResourcesController->appendResource(styleCreated);
        QPixmap pixmap(100, 100);
        pixmap.fill(styleCreated.getIconColor());
        stylesContextMenu->addStyle(styleCreated);
        delete this;
    });
}
