#include "textanalizer.h"

#include "extras/style.h"
#include "specialtexttag.h"

#include <QTextCursor>
#include <QGuiApplication>
#include <QCursor>
#include <QAction>
#include <QStandardItemModel>

TextAnalizer::TextAnalizer()
{
}

void TextAnalizer::writeToWord(OfficeLib::Word::Document& wordDocument, QTextCursor textCursor, const QList<Style>& styles, const QList<CustomImage>& importedImages, std::function<void(const Style*)> setTextPropertiesFunc)
{
    textCursor.movePosition(QTextCursor::End);
    int finalPos = textCursor.position();
    textCursor.movePosition(QTextCursor::Start);

    if (textCursor.position() == finalPos)
        return;

    QList<OfficeLib::Word::TableOfContents> tablesToUpdate;
    QColor col;
    do {
        if (textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor)) {
            col = textCursor.charFormat().background().color();

            QChar t = textCursor.selectedText().at(0);
            if (t == QChar(8233)) {       //Check for enter character (code 8233)
               wordDocument.writeText("\n");
            }

            const Style* curStyle = containsStyleByColor(col, styles);

            if (curStyle) {
                while (textCursor.charFormat().background().color() == col) {
                    if (textCursor.selectedText().endsWith("<")) {
                        textCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

                        QTextCursor specialTextCursor(textCursor);
                        specialTextCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
                        specialTextCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

                        if (containsSpecialText(specialTextCursor)) {
                            QString text = textCursor.selectedText();
                            if (!text.isEmpty()) {
                                text.replace(QChar(8233), "\n");

                                if (text.at(0) == '\n') {
                                    if (text.size() > 1)
                                        text.remove(0, 1);
                                    else
                                        text = "";
                                }

                                if (!text.isEmpty()) {
                                    setTextPropertiesFunc(curStyle);
                                    wordDocument.writeText(text);
                                }
                            }

                            SpecialTextTag specialTextTag(specialTextCursor.selectedText());
                            specialTextTag.doSomething(wordDocument, importedImages, tablesToUpdate);

                            textCursor = QTextCursor(specialTextCursor);
                            textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
                        }
                        else {
                            textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
                            textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
                        }
                    }
                    if (!textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor))
                        break;
                }

                QString text = textCursor.selectedText();
                if (!text.isEmpty()) {
                    text.replace(QChar(8233), "\n");

                    if (text.at(0) == '\n') {
                        if (text.size() > 1)
                            text.remove(0, 1);
                        else
                            text = "";
                    }

                    if (!text.isEmpty()) {
                        setTextPropertiesFunc(curStyle);
                        wordDocument.writeText(text);
                    }
                }
            }
        }
        textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
    } while (textCursor.position() < finalPos);

    for (OfficeLib::Word::TableOfContents& t : tablesToUpdate) {
        t.update();
    }
}

bool TextAnalizer::containsSpecialText(QTextCursor &cur)
{
    bool contains = false;

    for (int i = 0; i < 15; i++) {
        QString textSel = cur.selectedText();

        if (textSel.contains('>') && !textSel.contains('<')) {
            return false;
        }
        else if (textSel.contains('<')) {
            cur.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
            for (int j = 0; j < textSel.size(); j++){
                if (textSel[j] == '<')
                    break;
                cur.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
            }
            contains = true;
            break;
        }

        cur.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
    }

    if (!contains) {
        return false;
    }

    cur.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

    for (int i = 0; i < 15; i++) {
        cur.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        QString textSel = cur.selectedText();
        if (textSel.contains('>')) {
            for (int j = textSel.size() - 1; j >= 0; j--){
                textSel = cur.selectedText();
                if (textSel.endsWith('>'))
                    break;
                cur.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
            }
            break;
        }
    }

    const QString& selectedText = cur.selectedText();

    if (!selectedText.isEmpty() && selectedText.startsWith("<") && (selectedText.endsWith(">"))) {
        if (!specialTextSelectedCursor.selectedText().isEmpty()) {
            if (specialTextSelectedCursor.selectionStart() != cur.selectionStart() ||
                    specialTextSelectedCursor.selectionEnd() != cur.selectionEnd()) {
                removeUnderlinedSpecialText();
            }
            else
                return true;
        }
        if (!overridedCursor) {
            QGuiApplication::setOverrideCursor(Qt::PointingHandCursor);
            overridedCursor = true;
        }
        return true;
    }
    else {
        return false;
    }
}

const Style *TextAnalizer::containsStyleByColor(const QColor &col, const QList<Style> &styles) const
{
    for (int i = 1; i < styles.size(); i++) {
        if (col == styles[i].getIconColor())
            return &styles[i];
    }
    return nullptr;
}

const Style *TextAnalizer::containsStyleByName(const QString &name, const QList<Style> &styles) const
{
    for (int i = 1; i < styles.size(); i++) {
        if (name == styles[i].getName())
            return &styles[i];
    }
    return nullptr;
}

void TextAnalizer::removeUnderlinedSpecialText()
{
    if (!specialTextSelectedCursor.selectedText().isEmpty()) {
        QTextCharFormat charFormat = specialTextSelectedCursor.charFormat();
        charFormat.setFontUnderline(false);
        specialTextSelectedCursor.setCharFormat(charFormat);
        specialTextSelectedCursor = QTextCursor();
    }
}

void TextAnalizer::restoreOverridedCursor()
{
    if (overridedCursor) {
        QGuiApplication::restoreOverrideCursor();
        overridedCursor = false;
    }
}

void TextAnalizer::setTextSelectedColor(QTextCursor textCursor, const QList<Style> &styles, QAction* act)
{
    QColor col = act->icon().pixmap(100, 100).toImage().pixel(0, 0);

    QTextCharFormat charFormat = textCursor.charFormat();
    charFormat.setBackground(col);

    textCursor.beginEditBlock();
    textCursor.setCharFormat(charFormat);
    textCursor.endEditBlock();


    QString styleName = act->text();
    if (styleName == styles.at(0).getName())
        return; //TODO: Cuando se colorea un comentario entre 2 textos estilizados falta que se actualice la relacion

    TextRange newRange;
    newRange.start = textCursor.selectionStart();
    newRange.end = textCursor.selectionEnd();

    if (styleForRangeInText.contains(styleName)) {
        if (!checkAndResolveRangeCollision(styleName, newRange)) {
            bool newRangeAppended = false;
            for (int i = 0; i < styleForRangeInText[styleName].size(); i++) {
                if (newRange.start <= styleForRangeInText[styleName][i].start) {
                    styleForRangeInText[styleName].insert(i, newRange);
                    newRangeAppended = true;
                    break;
                }
            }
            if (!newRangeAppended)
                styleForRangeInText[styleName].append(newRange);
        }
    }
    else {
        QList<TextRange> ranges;
        ranges.append(newRange);
        styleForRangeInText.insert(styleName, ranges);
    }

    QList<QString> keys = styleForRangeInText.keys();
    for (int i = 0; i < keys.size(); i++) {
        QString debugText = keys[i] + ": ";
        for (int j = 0; j < styleForRangeInText[keys[i]].size(); j++) {
            debugText += QString("[%1, %2]%3").
                    arg(styleForRangeInText[keys[i]][j].start).
                    arg(styleForRangeInText[keys[i]][j].end).
                    arg(j == styleForRangeInText[keys[i]].size() - 1 ? "" : ", ");
        }
        qDebug() << debugText;
    }
}

void TextAnalizer::setNavegationTexts(QStandardItemModel *model, const QStandardItemModel* otherModel, QTextCursor textCursor)
{
    int maxStringLength = 30;

    const QString& stlFirstLevel = otherModel->item(0)->text();
    const QIcon& iconFirstLevel = otherModel->item(0)->icon();
    for (int i = 0; i < styleForRangeInText[stlFirstLevel].size(); i++) {
        int startIndex = styleForRangeInText[stlFirstLevel][i].start;
        int endIndex = styleForRangeInText[stlFirstLevel][i].end;
         endIndex = (endIndex - startIndex) + 1 <= maxStringLength ? endIndex : startIndex + maxStringLength - 1;

        textCursor.setPosition(startIndex, QTextCursor::MoveAnchor);
        textCursor.setPosition(endIndex, QTextCursor::KeepAnchor);
        QString textUnderCursor = textCursor.selectedText();

        QStandardItem* firstLevel = new QStandardItem(iconFirstLevel, textUnderCursor);
        firstLevel->setFlags(firstLevel->flags() ^ Qt::ItemIsEditable);
        model->appendRow(firstLevel);

        if (otherModel->rowCount() > 1)
            appendNextNavegationLevel(otherModel, firstLevel, 1, i, textCursor, endIndex, maxStringLength);
    }
}

void TextAnalizer::appendNextNavegationLevel(const QStandardItemModel *otherModel, QStandardItem* lastLevel, int levelIndex, int lastLevelItemIndex, QTextCursor& textCursor, int lastEndIndex, int maxStringLength)
{
    const QString& stlLevel = otherModel->item(levelIndex)->text();
    const QIcon& iconLevel = otherModel->item(levelIndex)->icon();

    for (int i = 0; i < styleForRangeInText[stlLevel].size(); i++) {
        if (styleForRangeInText[stlLevel][i].start > lastEndIndex) {
            if (styleForRangeInText[otherModel->item(levelIndex-1)->text()].size() < lastLevelItemIndex + 2 ||
                styleForRangeInText[stlLevel][i].start < styleForRangeInText[otherModel->item(levelIndex-1)->text()][lastLevelItemIndex + 1].start) {

                int startIndex = styleForRangeInText[stlLevel][i].start;
                int endIndex = styleForRangeInText[stlLevel][i].end;
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

void TextAnalizer::updateBackgroundStyleColor(const Style &style, const QList<Style> &styles, QTextCursor textCursor, const QString& previousName)
{
    QTextCharFormat charFormat = textCursor.charFormat();
    charFormat.setBackground(style.getIconColor());

    if (style.getName() != styles.at(0).getName()) {
        for (int i = 0; i < styleForRangeInText[previousName].size(); i++) {
            TextRange& currentRange = styleForRangeInText[previousName][i];
            textCursor.setPosition(currentRange.start, QTextCursor::MoveAnchor);
            textCursor.setPosition(currentRange.end, QTextCursor::KeepAnchor);
            textCursor.setCharFormat(charFormat);
        }
        styleForRangeInText.insert(style.getName(), styleForRangeInText[previousName]);
        styleForRangeInText.remove(previousName);
    }
    else {
        textCursor.setPosition(0, QTextCursor::MoveAnchor);
        textCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
        textCursor.setCharFormat(charFormat);

        QList<QString> keys = styleForRangeInText.keys();
        for (int i = 0; i < keys.size(); i++) {
            const Style* s = nullptr;
            for (int j = 0; j < styles.size(); j++) {
                if (styles.at(j).getName() == keys.at(i)) {
                    s = &styles.at(j);
                    break;
                }
            }
            if (s) {
                charFormat.setBackground(s->getIconColor());
                for (int j = 0; j < styleForRangeInText[keys[i]].size(); j++) {
                    TextRange& currentRange = styleForRangeInText[keys[i]][j];
                    textCursor.setPosition(currentRange.start, QTextCursor::MoveAnchor);
                    textCursor.setPosition(currentRange.end, QTextCursor::KeepAnchor);
                    textCursor.setCharFormat(charFormat);
                }
            }
        }
    }
}

bool TextAnalizer::checkAndResolveRangeCollision(const QString& styleName, TextRange& compareRange)
{
    for (int i = 0; i < styleForRangeInText[styleName].size(); i++) {
        TextRange& currentRange = styleForRangeInText[styleName][i];

        if (compareRange.end < currentRange.start - 1 || compareRange.start > currentRange.end + 1 ||
            (compareRange.start == currentRange.start && compareRange.end == currentRange.end))
            continue;

        int startDifference = currentRange.start - compareRange.start;
        int endDifference = currentRange.end - compareRange.end;

        auto destroyOtherSameRange = [&]() {
            for (int j = 0; j <  styleForRangeInText[styleName].size(); j++) {
                TextRange cRange = styleForRangeInText[styleName][j];
                if (cRange.start == compareRange.start && cRange.end == compareRange.end) {
                    styleForRangeInText[styleName].remove(j);
                }
            }
        };

        if (startDifference > 0) {
            int s = compareRange.start;
            destroyOtherSameRange();
            currentRange.start = s;
        }
        if (endDifference < 0) {
            int e = compareRange.end;
            destroyOtherSameRange();
            currentRange.end = e;
        }

        checkAndResolveRangeCollision(styleName, currentRange);
        return true;
    }
    return false;
}

void TextAnalizer::saveData(QDataStream &out)
{
    QList<QString> keys = styleForRangeInText.keys();

    out << (int)keys.size();
    for (int i = 0; i < keys.size(); i++) {
        out << keys[i];
        out << (int)styleForRangeInText[keys[i]].size();
        for (int j = 0; j < styleForRangeInText[keys[i]].size(); j++) {
            out << styleForRangeInText[keys[i]][j].start << styleForRangeInText[keys[i]][j].end;
        }
    }
}

void TextAnalizer::readData(QDataStream &in, const QList<Style> &styles, QTextCursor textCursor)
{
    int number;
    in >> number;

    for (int i = 0; i < number; i++) {
        QString key;
        int size;
        in >> key >> size;
        QList<TextRange> ranges;

        for (int j = 0; j < size; j++) {
            int start, end;
            in >> start >> end;
            TextRange range;
            range.start = start;
            range.end = end;
            ranges.append(range);
        }
        styleForRangeInText.insert(key, ranges);
    }

    QList<QString> keys = styleForRangeInText.keys();
    for (int i = 0; i < keys.size(); i++) {
        if (const Style* style = containsStyleByName(keys[i], styles)) {
            for (int j = 0; j < styleForRangeInText[keys[i]].size(); j++) {
                textCursor.setPosition(styleForRangeInText[keys[i]][j].start, QTextCursor::MoveAnchor);
                textCursor.setPosition(styleForRangeInText[keys[i]][j].end, QTextCursor::KeepAnchor);

                QTextCharFormat charFormat = textCursor.charFormat();
                charFormat.setBackground(style->getIconColor());
                textCursor.setCharFormat(charFormat);
            }
        }
    }
}
