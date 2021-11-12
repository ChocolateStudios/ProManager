#include "editstyledock.h"

#include "customs/customstylescontextmenu.h"
#include "customs/custompopupbutton.h"
#include "customs/customtextedit.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QAction>
#include <QCheckBox>

EditStyleDock::EditStyleDock(QMainWindow *mainWin, CustomStylesContextMenu* stylesContxtMenu, CustomTextEdit* txtEdit)
    : BaseDock(tr("Edit Style"), mainWin), stylesContextMenu(stylesContxtMenu), textEdit(txtEdit)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* widget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout;

    CustomPopupButton* selectedStyleBtn = new CustomPopupButton(tr("Select Style"), 175, 30);

    const QList<Style>& styles = stylesContextMenu->getStyles();
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
    QVector<OfficeLib::Word::WdColor> colorFonts = {
        OfficeLib::Word::wdColorAutomatic      ,
        OfficeLib::Word::wdColorBlack          ,
        OfficeLib::Word::wdColorBlue           ,
        OfficeLib::Word::wdColorTurquoise      ,
        OfficeLib::Word::wdColorBrightGreen    ,
        OfficeLib::Word::wdColorPink           ,
        OfficeLib::Word::wdColorRed            ,
        OfficeLib::Word::wdColorYellow         ,
        OfficeLib::Word::wdColorWhite          ,
        OfficeLib::Word::wdColorDarkBlue       ,
        OfficeLib::Word::wdColorTeal           ,
        OfficeLib::Word::wdColorGreen          ,
        OfficeLib::Word::wdColorViolet         ,
        OfficeLib::Word::wdColorDarkRed        ,
        OfficeLib::Word::wdColorDarkYellow     ,
        OfficeLib::Word::wdColorBrown          ,
        OfficeLib::Word::wdColorOliveGreen     ,
        OfficeLib::Word::wdColorDarkGreen      ,
        OfficeLib::Word::wdColorDarkTeal       ,
        OfficeLib::Word::wdColorIndigo         ,
        OfficeLib::Word::wdColorOrange         ,
        OfficeLib::Word::wdColorBlueGray       ,
        OfficeLib::Word::wdColorLightOrange    ,
        OfficeLib::Word::wdColorLime           ,
        OfficeLib::Word::wdColorSeaGreen       ,
        OfficeLib::Word::wdColorAqua           ,
        OfficeLib::Word::wdColorLightBlue      ,
        OfficeLib::Word::wdColorGold           ,
        OfficeLib::Word::wdColorSkyBlue        ,
        OfficeLib::Word::wdColorPlum           ,
        OfficeLib::Word::wdColorRose           ,
        OfficeLib::Word::wdColorTan            ,
        OfficeLib::Word::wdColorLightYellow    ,
        OfficeLib::Word::wdColorLightGreen     ,
        OfficeLib::Word::wdColorLightTurquoise ,
        OfficeLib::Word::wdColorPaleBlue       ,
        OfficeLib::Word::wdColorLavender       ,
        OfficeLib::Word::wdColorGray05         ,
        OfficeLib::Word::wdColorGray10         ,
        OfficeLib::Word::wdColorGray125        ,
        OfficeLib::Word::wdColorGray15         ,
        OfficeLib::Word::wdColorGray20         ,
        OfficeLib::Word::wdColorGray25         ,
        OfficeLib::Word::wdColorGray30         ,
        OfficeLib::Word::wdColorGray35         ,
        OfficeLib::Word::wdColorGray375        ,
        OfficeLib::Word::wdColorGray40         ,
        OfficeLib::Word::wdColorGray45         ,
        OfficeLib::Word::wdColorGray50         ,
        OfficeLib::Word::wdColorGray55         ,
        OfficeLib::Word::wdColorGray60         ,
        OfficeLib::Word::wdColorGray625        ,
        OfficeLib::Word::wdColorGray65         ,
        OfficeLib::Word::wdColorGray70         ,
        OfficeLib::Word::wdColorGray75         ,
        OfficeLib::Word::wdColorGray80         ,
        OfficeLib::Word::wdColorGray85         ,
        OfficeLib::Word::wdColorGray875        ,
        OfficeLib::Word::wdColorGray90         ,
        OfficeLib::Word::wdColorGray95         ,
    };

    for (int i = 0; i < styles.size() - 1; i++) {
        QAction* act = new QAction(styles[i].getName());
        QPixmap pixmap(100,100);
        pixmap.fill(styles[i].getIconColor());
        act->setIcon(QIcon(pixmap));

        connect(act, &QAction::triggered, this, [&]() {
            if (QAction* action = qobject_cast<QAction*>(sender())) {
                const Style* style = nullptr;
                for (const Style& s: stylesContextMenu->getStyles()) {
                    if (s.getName() == action->text()) {
                        style = &s;
                        break;
                    }
                }

                if (style) {
                    styleNameLEdit->setText(style->getName());
                    rectColorBtn->setStyleSheet("border: 1px solid black; background-color: " + style->getIconColor().name() + ";");
                    if (style->getName() != styles[0].getName()) {
                        FontColorBtn->setEnabled(true);
                        fontBtn->setEnabled(true);
                        if (style->getHeaderLevel() > 0) {
                            isHeaderCheckBox->setChecked(true);
                            emit headerLevelBtn->getMenuActions()[style->getHeaderLevel() - 1]->triggered();
                        }
//                        qDebug() << colorFonts.indexOf(style->getFontColor());
                        //                    FontColorBtn->getMenuAction()[]->triggered();
                    }
                    else {
                        FontColorBtn->setEnabled(false);
                        fontBtn->setEnabled(false);
                        isHeaderCheckBox->setEnabled(false);
                    }
                    styleUpdatingName = style->getName();
                }
            }
        });
        selectedStyleBtn->addActionToMenu(act);
    }

    QLabel* styleNameLabel = new QLabel(tr("Style Name"));
    styleNameLEdit = new QLineEdit;

    QHBoxLayout* rectColorLayout = new QHBoxLayout;
    QLabel* rectColorLabel = new QLabel(tr("Style Color"));
    rectColorBtn = new QToolButton();

    QVBoxLayout* fontPropsLayout = new QVBoxLayout;
    QLabel* fontLabel = new QLabel(tr("Font Properties"));
    QHBoxLayout* fontOptionsLayout = new QHBoxLayout;
    fontBtn = new QPushButton(tr("Font"));

    FontColorBtn = new CustomPopupButton("", 100);

    for (int i = 0; i < colorNames.size(); i++) {
        QAction* act = new QAction(colorNames[i]);
        connect(act, &QAction::triggered, stylesContextMenu, &CustomStylesContextMenu::setFontColorForUpdateStyle);
        FontColorBtn->addActionToMenu(act);
    }

    QHBoxLayout* headerOptions = new QHBoxLayout;
    isHeaderCheckBox = new QCheckBox(tr("is Header"));
    headerLevelBtn = new CustomPopupButton("");
    headerLevelBtn->setVisible(false);
    connect(isHeaderCheckBox, &QCheckBox::toggled, this, [&](bool checked) {
        if (!checked)
            stylesContextMenu->setHeaderLevelForUpdateStyle();
        headerLevelBtn->setVisible(checked);
    });

    for (int i = 0; i < 5; i++) {
        QAction* headerAct = new QAction(QString(tr("Header %1")).arg(i + 1));
        connect(headerAct, &QAction::triggered, stylesContextMenu, &CustomStylesContextMenu::setHeaderLevelForUpdateStyle);
        headerLevelBtn->addActionToMenu(headerAct);
    }

    updateStyleBtn = new QPushButton("Save Changes");

    connect(rectColorBtn, &QToolButton::clicked, stylesContextMenu, &CustomStylesContextMenu::setIconColorForUpdateStyle);
    connect(fontBtn, &QPushButton::clicked, stylesContextMenu, &CustomStylesContextMenu::setFontForUpdateStyle);
    connect(updateStyleBtn, &QPushButton::clicked, this, [&]() {
        stylesContextMenu->updateStyleFromDock(styleUpdatingName, updateStyleBtn, textEdit);
        autoDestroy();
    });

    QWidget* verticalSpacer = new QWidget;
    verticalSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addWidget(selectedStyleBtn);
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

    layout->addWidget(updateStyleBtn);
    layout->addWidget(verticalSpacer);

    widget->setLayout(layout);

    setWidget(widget);

    mainWin->addDockWidget(Qt::RightDockWidgetArea, this);
}
