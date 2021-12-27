#include "textanalizer.h"

#include "docks/dockcontroller.h"

#include "resources/controllers/styleresourcescontroller.h"
#include "resources/styleresource.h"

#include "editors/textdocumenteditor.h"

#include "analizers/tag/taganalizer.h"

#include "textrange/textrange.h"

#include <QGuiApplication>
#include <QStandardItemModel>

TextAnalizer::TextAnalizer(StyleResourcesController* styleResourcesController)
    : styleResourcesController(styleResourcesController)
{
}

void TextAnalizer::colorSelectedText(const StyleResource &newStyle)
{
    if (!activeEditor) return;

    if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {

        QTextCursor cur(textDocumentEditor->textCursor());
        QTextCharFormat charFormat =  cur.charFormat();
        charFormat.setBackground(newStyle.getIconColor());
        cur.setCharFormat(charFormat);

        TextRange newTextRange(cur.selectionStart(), cur.selectionEnd());
        textDocumentEditor->textRangesController.appendTextRange(newStyle.getName(), newTextRange);

        textDocumentEditor->textRangesController.debugTextRanges();
    }
}

void TextAnalizer::updateStyleColorInEditors(const QString &oldStyleName, const StyleResource &newStyle)
{
    if (!activeEditor) return;

//    if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
//        QTextCursor cur(textDocumentEditor->textCursor());

//        QTextCharFormat charFormat = cur.charFormat();
//        charFormat.setBackground(newStyle.getIconColor());

//        if (newStyle.getName() != styleResourcesController->getCommentStyleName()) {
//            int size = textDocumentEditor->styleRanges[oldStyleName].size();
//            for (int i = 0; i < size; i++) {
//                TextRange& currentRange = textDocumentEditor->styleRanges[oldStyleName][i];
//                cur.setPosition(currentRange.start, QTextCursor::MoveAnchor);
//                cur.setPosition(currentRange.start + currentRange.lenght, QTextCursor::KeepAnchor);
//                cur.setCharFormat(charFormat);
//            }
//            textDocumentEditor->styleRanges.insert(newStyle.getName(), textDocumentEditor->styleRanges[oldStyleName]);
//            textDocumentEditor->styleRanges.remove(oldStyleName);
//        }
//        else {
//            cur.setPosition(0, QTextCursor::MoveAnchor);
//            cur.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
//            cur.setCharFormat(charFormat);

//            QList<QString> keys = textDocumentEditor->styleRanges.keys();
//            int size = keys.size();
//            for (int i = 0; i < size; i++) {
//                const StyleResource* stl = styleResourcesController->getStyleByName(keys.at(i));

//                if (stl) {
//                    charFormat.setBackground(stl->getIconColor());
//                    int size2 = textDocumentEditor->styleRanges[keys[i]].size();
//                    for (int j = 0; j < size2; j++) {
//                        TextRange& currentRange = textDocumentEditor->styleRanges[keys[i]][j];
//                        cur.setPosition(currentRange.start, QTextCursor::MoveAnchor);
//                        cur.setPosition(currentRange.start + currentRange.lenght, QTextCursor::KeepAnchor);
//                        cur.setCharFormat(charFormat);
//                    }
//                }
//            }
//        }
//    }
}

QStandardItemModel *TextAnalizer::getNavegationModel(const QStandardItemModel* styleHierarchyModel)
{
    if (!activeEditor) return nullptr;

    int maxStringLength = 30;

    QStandardItemModel* model = new QStandardItemModel(0, 1);
    model->setHeaderData(0, Qt::Horizontal, "documentName");

    const QString& stlFirstLevel = styleHierarchyModel->item(0)->text();
    const QIcon& iconFirstLevel = styleHierarchyModel->item(0)->icon();

    if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
        QTextCursor cur(textDocumentEditor->textCursor());
        const QList<TextRange>& styleRanges = textDocumentEditor->textRangesController.getTextRangesByStyleName(stlFirstLevel);
        int size = styleRanges.size();
        for (int i = 0; i < size; i++) {
            int startIndex = styleRanges[i].start;
            int endIndex = styleRanges[i].end();
             endIndex = (endIndex - startIndex) + 1 <= maxStringLength ? endIndex : startIndex + maxStringLength - 1;

            cur.setPosition(startIndex, QTextCursor::MoveAnchor);
            cur.setPosition(endIndex, QTextCursor::KeepAnchor);
            QString textUnderCursor = cur.selectedText();

            QStandardItem* firstLevel = new QStandardItem(iconFirstLevel, textUnderCursor);
            firstLevel->setFlags(firstLevel->flags() ^ Qt::ItemIsEditable);
            model->appendRow(firstLevel);

            if (styleHierarchyModel->rowCount() > 1)
                appendNextNavegationLevel(styleHierarchyModel, firstLevel, 1, i, cur, endIndex, maxStringLength);
        }
    }

    return model;
}

void TextAnalizer::appendNextNavegationLevel(const QStandardItemModel *otherModel, QStandardItem* lastLevel, int levelIndex, int lastLevelItemIndex, QTextCursor& textCursor, int lastEndIndex, int maxStringLength)
{
    const QString& stlLevel = otherModel->item(levelIndex)->text();
    const QIcon& iconLevel = otherModel->item(levelIndex)->icon();


    if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
        TextRangesController& txRngsController = textDocumentEditor->textRangesController;
        const QList<TextRange>& styleRanges = txRngsController.getTextRangesByStyleName(stlLevel);
        int size = styleRanges.size();

        for (int i = 0; i < size; i++) {
            if (styleRanges[i].start > lastEndIndex) {
                if (txRngsController.getTextRangesByStyleName(otherModel->item(levelIndex-1)->text()).size() < lastLevelItemIndex + 2 ||
                        styleRanges[i].start < txRngsController.getTextRangesByStyleName(otherModel->item(levelIndex-1)->text())[lastLevelItemIndex + 1].start) {

                    int startIndex = styleRanges[i].start;
                    int endIndex = styleRanges[i].end();
                    endIndex = (endIndex - startIndex) + 1 <= maxStringLength ? endIndex : startIndex + maxStringLength - 1;

                    textCursor.setPosition(startIndex, QTextCursor::MoveAnchor);
                    textCursor.setPosition(endIndex, QTextCursor::KeepAnchor);
                    const QString& textUnderCursor = textCursor.selectedText();

                    QStandardItem* level = new QStandardItem(iconLevel, textUnderCursor);
                    level->setFlags(level->flags() ^ Qt::ItemIsEditable);
                    lastLevel->appendRow(level);

                    if (levelIndex < otherModel->rowCount() - 1)
                        appendNextNavegationLevel(otherModel, level, levelIndex + 1, i, textCursor, endIndex, maxStringLength);
                }
            }
        }
    }
}

void TextAnalizer::setActiveEditor(BaseEditor *editor)
{
    activeEditor = editor;
}

void TextAnalizer::searchTagInEditor()
{
    if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
        QTextCursor cur = textDocumentEditor->cursorForPosition(textDocumentEditor->mapFromGlobal(QCursor::pos()));
        cur.select(QTextCursor::WordUnderCursor);

        if (TagAnalizer::containsTag(cur)) {
            removeUnderlinedTagText();
            QTextCharFormat charFormat = cur.charFormat();
            charFormat.setFontUnderline(true);
            cur.setCharFormat(charFormat);
            tagTextSelectedCursor = cur;
            overrideCursor();
        }
        else {
            restoreOverridedCursor();
            removeUnderlinedTagText();
        }
    }
}

void TextAnalizer::openDockByTagType()
{
    const QString& tagText = tagTextSelectedCursor.selectedText();
    if (tagText.isEmpty()) return;

    TagAnalizer analizer(tagText);
    int dockId = analizer.getDockIdByType();

    if (dockId != -1) {
        if (!dockController->isDockVisible(dockId))
            dockController->createOrDestroyDockById(dockId);
    }
}

void TextAnalizer::stopSearchingTagText()
{
    restoreOverridedCursor();
    removeUnderlinedTagText();
}

void TextAnalizer::onColorSelectedText(const StyleResource &newStyle)
{
    colorSelectedText(newStyle);
}

void TextAnalizer::overrideCursor()
{
    if (!cursorOverrided) {
        QGuiApplication::setOverrideCursor(Qt::PointingHandCursor);
        cursorOverrided = true;
    }
}

void TextAnalizer::restoreOverridedCursor()
{
    if (cursorOverrided) {
        QGuiApplication::restoreOverrideCursor();
        cursorOverrided = false;
    }
}

void TextAnalizer::removeUnderlinedTagText()
{
    if (!tagTextSelectedCursor.selectedText().isEmpty()) {
        QTextCharFormat charFormat = tagTextSelectedCursor.charFormat();
        charFormat.setFontUnderline(false);
        tagTextSelectedCursor.setCharFormat(charFormat);
        tagTextSelectedCursor = QTextCursor();
    }
}
