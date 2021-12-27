#include "styleeditordock.h"

#include "docks/dockarguments.h"

#include "resources/controllers/styleresourcescontroller.h"

#include "menus/stylescontextmenu.h"

#include "word/wordcolorfontslist.h"

#include "widgets/popupbutton.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QAction>
#include <QCheckBox>
#include <QColorDialog>
#include <QFontDialog>

StyleEditorDock::StyleEditorDock(DockArguments args)
    : BaseDock(args.mainWin, tr("Edit Style")), StyleResourcesObserver(args.get<QList<StyleResource>>(0)),
      styleResourcesController(args.get<StyleResourcesController>(1)),
      stylesContextMenu(args.get<StylesContextMenu>(2))
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Widgets

    QWidget* widget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout;

    PopupButton* selectedStyleBtn = new PopupButton(tr("Select Style"), 175, 30);

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
    PopupButton* headerLevelBtn = new PopupButton;
    headerLevelBtn->setVisible(false);

    QPushButton* updateStyleBtn = new QPushButton("Save Changes");

    QWidget* verticalSpacer = new QWidget;
    verticalSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //Layout

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

    args.mainWin->addDockWidget(Qt::RightDockWidgetArea, this);

    // Connections

    int size = styles->size();

    for (int i = 0; i < size; i++) {
        QAction* act = new QAction(styles->at(i).getName());
        QPixmap pixmap(100,100);
        pixmap.fill(styles->at(i).getIconColor());
        act->setIcon(QIcon(pixmap));

        connect(act, &QAction::triggered, this, [&]() {
            if (QAction* action = qobject_cast<QAction*>(sender())) {
                const StyleResource* style = styleResourcesController->getStyleByName(action->text());

                if (style) {
                    indexUpdated = ((PopupButton*)this->widget()->children()[1])->getMenuActions().indexOf(action);

                    ((QLineEdit*)this->widget()->children()[3])->setText(style->getName());
                    ((QToolButton*)this->widget()->children()[5])->setStyleSheet("border: 1px solid black; background-color: " + style->getIconColor().name() + ";");

                    if (style->getName() != styleResourcesController->getCommentStyleName()) {
                        ((PopupButton*)this->widget()->children()[8])->setEnabled(true);
                        ((QPushButton*)this->widget()->children()[7])->setEnabled(true);
                        if (style->getHeaderLevel() > 0) {
                            ((QCheckBox*)this->widget()->children()[9])->setChecked(true);
                            emit ((PopupButton*)this->widget()->children()[10])->getMenuActions()[style->getHeaderLevel() - 1]->triggered();
                        }
                    }
                    else {
                        ((PopupButton*)this->widget()->children()[8])->setEnabled(false);
                        ((QPushButton*)this->widget()->children()[7])->setEnabled(false);
                        ((QCheckBox*)this->widget()->children()[9])->setEnabled(false);
                    }
                }
            }
        });
        selectedStyleBtn->addActionToMenu(act);
    }


    size = OfficeLib::Word::colorNames.size();
    for (int i = 0; i < size; i++) {
        QAction* act = new QAction(OfficeLib::Word::colorNames[i]);
        connect(act, &QAction::triggered, this, [&]() {
            QAction* act = qobject_cast<QAction*>(sender());
            OfficeLib::Word::WdColor newFontColor = OfficeLib::Word::colorFonts[OfficeLib::Word::colorNames.indexOf(act->text())];
            styleUpdated.setFontColor(newFontColor);
        });
        FontColorBtn->addActionToMenu(act);
    }


    connect(isHeaderCheckBox, &QCheckBox::toggled, this, [&](bool checked) {
        if (!checked)
            styleUpdated.setHeaderLevel(0);
        ((PopupButton*)this->widget()->children()[10])->setVisible(checked);
    });

    for (int i = 1; i < 6; i++) {
        QAction* headerAct = new QAction(QString(tr("Header %1")).arg(i));
        connect(headerAct, &QAction::triggered, this, [&]() {
            QAction* act = qobject_cast<QAction*>(sender());
            bool ok = false;
            unsigned short newHeaderLevel = QString(act->text().at(act->text().size() - 1)).toShort(&ok);
            if (ok)
                styleUpdated.setHeaderLevel(newHeaderLevel);
        });
        headerLevelBtn->addActionToMenu(headerAct);
    }


    connect(rectColorBtn, &QToolButton::clicked, this, [&]() {
        QToolButton* btn = qobject_cast<QToolButton*>(sender());
        QColor newIconColor = QColorDialog::getColor();
        btn->setStyleSheet("border: 1px solid black; background-color: " + newIconColor.name() + ";");
        styleUpdated.setIconColor(newIconColor);
    });

    connect(fontBtn, &QPushButton::clicked, this, [&]() {
        QPushButton* btn = qobject_cast<QPushButton*>(sender());
        bool ok = false;
        QFont newFont = QFontDialog::getFont(&ok);
        if (ok) {
            styleUpdated.setFont(newFont);
            QFont btnFont = btn->font();
            btnFont.setBold(true);
            btn->setFont(btnFont);
        }
    });

    connect(updateStyleBtn, &QPushButton::clicked, this, [&]() {
        styleUpdated.setName(((QLineEdit*)this->widget()->children()[3])->text());

        if (styleUpdated.getName().isEmpty())
            return;

        styleResourcesController->updateResource(indexUpdated, styleUpdated);
        QPixmap pixmap(100, 100);
        pixmap.fill(styleUpdated.getIconColor());
        stylesContextMenu->updateStyle(indexUpdated, styleUpdated);
        delete this;
    });
}

void StyleEditorDock::update()
{
    qDebug() << "must update styles in style editor dock";
}
