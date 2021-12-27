#include "taganalizer.h"

#include "converters/wordconverter.h"
#include "converters/powerpointconverter.h"
#include "converters/excelconverter.h"

#include <QTextCursor>

TagAnalizer::TagAnalizer(const QString &tagText)
{
    readText(tagText);
}

TagAnalizer::TagAnalizer(BaseConverter* converter, ResourcesManager *resManager)
    : converter(converter), resManager(resManager)
{

}

void TagAnalizer::doSomething(const QString& tagText)
{
    readText(tagText);

    if (WordConverter* wordConverter = dynamic_cast<WordConverter*>(converter))
        doSomethingInWord(wordConverter);

    else if (ExcelConverter* excelConverter = dynamic_cast<ExcelConverter*>(converter))
        doSomethingInExcel(excelConverter);
}

void TagAnalizer::readText(const QString &text)
{
    clearText();
    unsigned int i = 1;
    jumpToNextWord(i, text);
    readType(i, text);
    jumpToNextWord(i, text);

    unsigned int size = text.size();
    while (i < size - 1) {
        readProperty(i, text);
        jumpToNextWord(i, text);
    }
}

int TagAnalizer::getDockIdByType()
{
    QList<QString> types = {"☺", "☺", "img", "file"};
    int size = types.size();

    for (int i = 0; i < size; i++) {
        if (type == types[i])
            return i;
    }
    return -1;
}

const QString &TagAnalizer::getPropertyValue(const QString &propertyKey)
{
    return properties[propertyKey];
}

void TagAnalizer::clearText()
{
    type.clear();
    properties.clear();
}

void TagAnalizer::jumpToNextWord(unsigned int &index, const QString &text)
{
    while (index < text.size() - 1 && (text[index] == ' ' || text[index] == QChar(8233))) { //8233 -> \n
        index++;
    }
}

void TagAnalizer::readType(unsigned int &index, const QString &text)
{
    while (index < text.size() - 1 && text[index] != ' ') {
        type += text[index];
        index++;
    }
    type = type.toLower();
}

void TagAnalizer::readProperty(unsigned int &index, const QString &text)
{
    QString propKey = "";
    QString propValue = "";

    while (index < text.size() - 1 && text[index] != ' ' && text[index] != '=') {
        propKey += text[index];
        index++;
    }

    jumpToNextWord(index, text);

    if (text[index] == '=') {
        index++;
        jumpToNextWord(index, text);
    }
    else
        return; //Maybe throw error

    if (text[index] == '\"')
        index++;
    else
        return;

    bool propertyFinished = false;
    while (index < text.size() - 1) {
        if (text[index] == '\"') {
            index++;
            propertyFinished = true;
            break;
        }
        propValue += text[index];
        index++;
    }

    if (propKey.isEmpty() || propValue.isEmpty() || !propertyFinished)
        return;

    propKey = propKey.toLower();

    properties.insert(propKey, propValue);
}

bool TagAnalizer::containsTag(QTextCursor &textCursor)
{
    const QString& mustContainsText = textCursor.selectedText();
    if (mustContainsText.isEmpty()) return false;

    bool contains = false;
    const int maxTagLength = 200;

    for (int i = 0; i < maxTagLength; i++) {
        QString textSel = textCursor.selectedText();

        if (textSel.contains('>') && !textSel.contains('<')) {
            return false;
        }
        else if (textSel.contains('<')) {
            textCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
            for (int j = 0; j < textSel.size(); j++){
                if (textSel[j] == '<')
                    break;
                textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
            }
            contains = true;
            break;
        }

        textCursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
    }

    if (!contains) {
        return false;
    }

    textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

    for (int i = 0; i < maxTagLength; i++) {
        textCursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        QString textSel = textCursor.selectedText();
        if (textSel.contains('>')) {
            for (int j = textSel.size() - 1; j >= 0; j--){
                textSel = textCursor.selectedText();
                if (textSel.endsWith('>'))
                    break;
                textCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
            }
            break;
        }
    }

    const QString& selectedText = textCursor.selectedText();

    if (!selectedText.isEmpty() && selectedText.contains(mustContainsText) && selectedText.startsWith("<") && (selectedText.endsWith(">"))) {
//        if (!specialTextSelectedCursor.selectedText().isEmpty()) {
//            if (specialTextSelectedCursor.selectionStart() != textCursor.selectionStart() ||
//                    specialTextSelectedCursor.selectionEnd() != textCursor.selectionEnd()) {
//                removeUnderlinedSpecialText();
//            }
//            else
//                return true;
//        }
//        if (!overridedCursor) {
//            QGuiApplication::setOverrideCursor(Qt::PointingHandCursor);
//            overridedCursor = true;
//        }
        return true;
    }
    else {
        return false;
    }
}
