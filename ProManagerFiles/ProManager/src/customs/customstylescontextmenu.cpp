#include "customs/customstylescontextmenu.h"
#include "customs/customaction.h"
#include "customs/customtextedit.h"

#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>

CustomStylesContextMenu::CustomStylesContextMenu(CustomTextEdit* textEdit, QWidget* parent)
    : QMenu(parent), textEdit(textEdit)
{
    styles.append(Style());
}

const QList<Style> &CustomStylesContextMenu::getStyles() const
{
    return styles;
}

void CustomStylesContextMenu::setAddStyleAction(QAction* action)
{
    //addStyleAct = action;
    //addStyleAct->setIcon(QIcon(":/images/plus.png"));
    addAction(action);
    addSeparator();
}

void CustomStylesContextMenu::customPopup()
{
    popup(parentWidget()->mapToGlobal(QPoint(textEdit->cursorRect().right() + 15, textEdit->cursorRect().bottom() + 60)));
}

void CustomStylesContextMenu::addStyle()
{
    static QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    static QLineEdit* ledit = qobject_cast<QLineEdit*>(btn->parent()->children().at(2));

    if (ledit) {
        for (Style &s : styles) {
            if (s.getName() == ledit->text())
                return;
        }

        styles.last().setName(ledit->text());

        CustomAction* newAct = new CustomAction(styles.last());
        newAct->setEnabled(!textEdit->textCursor().selectedText().isEmpty());

        connect(newAct, &QAction::triggered, textEdit, &CustomTextEdit::setTextSelectedColor);
        connect(textEdit, &QTextEdit::copyAvailable, newAct, &QAction::setEnabled);

        addAction(newAct);

        ledit->clear();
        btn->parent()->parent()->setProperty("visible", false);
        styles.append(Style());

        if (!textEdit->textCursor().selectedText().isEmpty())
            emit newAct->triggered();
    }
}

void CustomStylesContextMenu::setIconColorForNewStyle()
{
    if (QToolButton* btn = qobject_cast<QToolButton*>(sender())) {
        styles.last().setIconColor(btn);
    }
}

void CustomStylesContextMenu::setFontForNewStyle()
{
    styles.last().setFont();
}

void CustomStylesContextMenu::setFontColorForNewStyle()
{
    if (QToolButton* btn = qobject_cast<QToolButton*>(sender())) {
        styles.last().setFontColor(btn);
    }
}
