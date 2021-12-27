#include "stylescontextmenu.h"

#include "resources/controllers/styleresourcescontroller.h"

#include "editors/baseeditor.h"
#include "editors/textdocumenteditor.h"

#include "analizers/textanalizer.h"

#include <QMainWindow>

StylesContextMenu::StylesContextMenu(QMainWindow *win, TextAnalizer* analizer, StyleResourcesController* stylesController)
    : mainWin(win), textAnalizer(analizer), styleResourcesController(stylesController)
{
}

void StylesContextMenu::addStyle(const StyleResource &style)
{
    QPixmap iconPixmap(100, 100);
    iconPixmap.fill(style.getIconColor());

    QAction* newAct = new QAction(style.getName());
    newAct->setIcon(QIcon(iconPixmap));

    addAction(newAct);
    stylesCount++;

    if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
        newAct->setEnabled(textDocumentEditor->textCursor().hasSelection());

        connect(newAct, &QAction::triggered, this, [&]() {
            QAction* act = qobject_cast<QAction*>(sender());
            const StyleResource* style = styleResourcesController->getStyleByName(act->text());
            if (style)
                textAnalizer->colorSelectedText(*style);
        });

        connect(textDocumentEditor, &QTextEdit::copyAvailable, newAct, &QAction::setEnabled);

        if (textDocumentEditor->textCursor().hasSelection())
            emit newAct->triggered();
    }
    else {
        newAct->setEnabled(false);

        connect(newAct, &QAction::triggered, this, [&]() {
            QAction* act = qobject_cast<QAction*>(sender());
            const StyleResource* style = styleResourcesController->getStyleByName(act->text());
            if (style)
                textAnalizer->colorSelectedText(*style);
        });
    }

//    int size = actions().size();
//    for(int i = 0; i < size; i++) {
//        qDebug() << actions()[i];
//    }
}

void StylesContextMenu::updateStyle(const unsigned int &index, const StyleResource &newStyle)
{
    QPixmap iconPixmap(100, 100);
    iconPixmap.fill(newStyle.getIconColor());

    QAction* act = actions()[index];
    const QString& oldStyleName = act->text();

    act->setText(newStyle.getName());
    act->setIcon(QIcon(iconPixmap));

    textAnalizer->updateStyleColorInEditors(oldStyleName, newStyle);
}

void StylesContextMenu::removeStyle(const unsigned int &index)
{
    QAction* act = actions()[index];
    removeAction(act);
    delete act;
    stylesCount--;
}

void StylesContextMenu::clearStyles()
{
    for (int i = 0; i < stylesCount; i++) {
        removeStyle(i + stylesStartIndex);
    }
}

void StylesContextMenu::setActiveEditor(BaseEditor *editor)
{
    if (TextDocumentEditor* newTextDocumentEditor = dynamic_cast<TextDocumentEditor*>(editor)) {
        TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor);

        for (int i = 0; i < stylesCount; i++) {
            QAction* act = actions()[i + stylesStartIndex];
            if (textDocumentEditor)
                disconnect(textDocumentEditor, &QTextEdit::copyAvailable, act, &QAction::setEnabled);

            connect(newTextDocumentEditor, &QTextEdit::copyAvailable, act, &QAction::setEnabled);
        }
    }
    activeEditor = editor;
}

void StylesContextMenu::popupInSelection()
{
    if (!activeEditor) return;

    if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
        popup(mainWin->mapToGlobal(QPoint(textDocumentEditor->cursorRect().right() + 13, textDocumentEditor->cursorRect().bottom() + 80)));
    }
}
