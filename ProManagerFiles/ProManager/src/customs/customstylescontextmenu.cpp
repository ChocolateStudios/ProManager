#include "customs/customstylescontextmenu.h"
#include "customs/customaction.h"
#include "customs/customtextedit.h"
#include "customs/customcolorfontdialog.h"

#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QColorDialog>
#include <QFontDialog>

CustomStylesContextMenu::CustomStylesContextMenu(CustomTextEdit* textEdit, QWidget* parent)
    : QMenu(parent), textEdit(textEdit)
{
}

const QList<Style> &CustomStylesContextMenu::getStyles() const
{
    return styles;
}

void CustomStylesContextMenu::setAddStyleAction(QAction* action)
{
    addAction(action);
    addSeparator();

    Style commentStyle;
    commentStyle.setName(tr("Comment"));
    commentStyle.setIconColor(Qt::white);
    styles.append(commentStyle);
    addStyle();
}

void CustomStylesContextMenu::saveData(QDataStream &out)
{
    out << (int)styles.size();
    for (int i = 1; i < styles.size() - 1; i++) {
        out << styles[i].getFont() << styles[i].getFontColor()
            << styles[i].getIconColor() << styles[i].getName()
            << styles[i].getHeaderLevel();
    }
}

void CustomStylesContextMenu::readData(QDataStream &in)
{
    int number;
    in >> number;

    for (int i = 1; i < number - 1; i++) {
        QFont font;
        OfficeLib::Word::WdColor fontColor;
        QColor iconColor;
        QString name;
        unsigned short headerLevel;
        in >> font >> fontColor >> iconColor >> name >> headerLevel;

        styles.last().setFont(font);
        styles.last().setFontColor(fontColor);
        styles.last().setIconColor(iconColor);
        styles.last().setName(name);
        styles.last().setHeaderLevel(headerLevel);
        addStyle();
    }
}

void CustomStylesContextMenu::customPopup()
{
    popup(parentWidget()->mapToGlobal(QPoint(textEdit->cursorRect().right() + 15, textEdit->cursorRect().bottom() + 60)));
}

void CustomStylesContextMenu::addStyleFromDock()
{
    if (QPushButton* btn = qobject_cast<QPushButton*>(sender())) {
        if (QLineEdit* ledit = qobject_cast<QLineEdit*>(btn->parent()->children().at(2))) {

            for (Style &s : styles) {
                if (s.getName() == ledit->text())
                    return;
            }

            styles.last().setName(ledit->text());

            addStyle();
        }
    }
}

void CustomStylesContextMenu::updateStyleFromDock(QString styleName, QPushButton* btn, CustomTextEdit* textEdit)
{
    if (QLineEdit* ledit = qobject_cast<QLineEdit*>(btn->parent()->children().at(3))) {
        Style* style = nullptr;

        int i = 0;
        for (; i < styles.size() - 1; i++) {
            if (styles[i].getName() == styleName) {
                style = &styles[i];
                break;
            }
        }

        if (style) {
            styles.last().setName(ledit->text());
            i+=2;
            updateStyle(*style, i, textEdit);
        }
    }
}

void CustomStylesContextMenu::setIconColorForNewStyle()
{
    if (QToolButton* btn = qobject_cast<QToolButton*>(sender())) {
        QColor newIconColor = QColorDialog::getColor();
        btn->setStyleSheet("border: 1px solid black; background-color: " + newIconColor.name() + ";");
        styles.last().setIconColor(newIconColor);
    }
}

void CustomStylesContextMenu::setFontForNewStyle()
{
    bool ok = false;
    QFont newFont = QFontDialog::getFont(&ok);

    if (ok)
        styles.last().setFont(newFont);
}

static QVector<QString> colorNames = {
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
static QVector<OfficeLib::Word::WdColor> colorFonts = {
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

void CustomStylesContextMenu::setFontColorForNewStyle()
{
    if (QAction* act = qobject_cast<QAction*>(sender())) {
        OfficeLib::Word::WdColor newFontColor = colorFonts[colorNames.indexOf(act->text())];
        styles.last().setFontColor(newFontColor);
    }
}

void CustomStylesContextMenu::setHeaderLevelForNewStyle()
{
    if (QAction* act = qobject_cast<QAction*>(sender())) {
        bool ok = false;
        unsigned short newHeaderLevel = QString(act->text().at(act->text().size() - 1)).toShort(&ok);
        if (ok) {
            styles.last().setHeaderLevel(newHeaderLevel);
        }
    }
    else {
        styles.last().setHeaderLevel(0);
    }
}

void CustomStylesContextMenu::setIconColorForUpdateStyle()
{
    if (QToolButton* btn = qobject_cast<QToolButton*>(sender())) {
        QColor newIconColor = QColorDialog::getColor();
        btn->setStyleSheet("border: 1px solid black; background-color: " + newIconColor.name() + ";");
        styles.last().setIconColor(newIconColor);
    }
}

void CustomStylesContextMenu::setFontForUpdateStyle()
{
    bool ok = false;
    QFont newFont = QFontDialog::getFont(&ok);

    if (ok)
        styles.last().setFont(newFont);
}

void CustomStylesContextMenu::setFontColorForUpdateStyle()
{
    if (QAction* act = qobject_cast<QAction*>(sender())) {
        OfficeLib::Word::WdColor newFontColor = colorFonts[colorNames.indexOf(act->text())];
        styles.last().setFontColor(newFontColor);
    }
}

void CustomStylesContextMenu::setHeaderLevelForUpdateStyle()
{
    if (QAction* act = qobject_cast<QAction*>(sender())) {
        bool ok = false;
        unsigned short newHeaderLevel = QString(act->text().at(act->text().size() - 1)).toShort(&ok);
        if (ok) {
            styles.last().setHeaderLevel(newHeaderLevel);
        }
    }
    else {
        styles.last().setHeaderLevel(0);
    }
}

void CustomStylesContextMenu::addStyle()
{
    CustomAction* newAct = new CustomAction(styles.last());
    newAct->setEnabled(!textEdit->textCursor().selectedText().isEmpty());

    connect(newAct, &QAction::triggered, textEdit, &CustomTextEdit::setTextSelectedColor);
    connect(textEdit, &QTextEdit::copyAvailable, newAct, &QAction::setEnabled);

    addAction(newAct);

    styles.append(Style());

    if (!textEdit->textCursor().selectedText().isEmpty())
        emit newAct->triggered();
}

void CustomStylesContextMenu::updateStyle(Style& style, int index, CustomTextEdit* textEdit)
{
    QString previousName = style.getName();
    style.setName(styles.last().getName());
    style.setIconColor(styles.last().getIconColor());
    style.setFont(styles.last().getFont());
    style.setFontColor(styles.last().getFontColor());
    style.setHeaderLevel(styles.last().getHeaderLevel());

    styles.last() = Style();

    actions()[index]->setText(style.getName());

    QPixmap pixmap(100,100);
    pixmap.fill(style.getIconColor());

    actions()[index]->setIcon(QIcon(pixmap));

    textEdit->updateBackgroundStyleColor(previousName, style);
}
