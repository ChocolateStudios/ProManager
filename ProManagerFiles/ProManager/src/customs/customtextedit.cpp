#include "customs/customtextedit.h"

#include "word/document.h"
#include "extras/style.h"

#include <QtWidgets>
#include <QString>

CustomTextEdit::CustomTextEdit()
{
    setAcceptRichText(false);
    setMouseTracking(true);
}

void CustomTextEdit::setStyles(const QList<Style> *stls)
{
    styles = stls;
}

void CustomTextEdit::writeToWord(OfficeLib::Word::Document& wordDocument, const QList<Style>& styles, const QList<CustomImage>& importedImages)
{
    QTextCursor cur(textCursor());
    textAnalizer.writeToWord(wordDocument, cur, styles, importedImages,
        [&](const Style* curStyle) {

            //Font Properties
            QFont font = curStyle->getFont();
            OfficeLib::Word::WdColor fontColor = curStyle->getFontColor();
            unsigned short headerLevel = curStyle->getHeaderLevel();

            wordDocument.setHeader(headerLevel);
            wordDocument.setFontColor(fontColor);
            wordDocument.setFontBold(font.bold());
            wordDocument.setFontName(font.family());
            wordDocument.setFontSize(font.pointSize());

            //Paragraph Properties
            OfficeLib::Word::WdParagraphAlignment alignment = OfficeLib::Word::wdAlignParagraphJustify;
            wordDocument.setParagraphAlignment(alignment);
        }
    );
}

void CustomTextEdit::setNavegationTexts(QStandardItemModel* model, const QStandardItemModel* otherModel)
{
    textAnalizer.setNavegationTexts(model, otherModel, textCursor());
}

void CustomTextEdit::updateBackgroundStyleColor(const QString& previousName, const Style& style)
{
    textAnalizer.updateBackgroundStyleColor(style, *styles, textCursor(), previousName);
}

void CustomTextEdit::saveData(QDataStream &out)
{
    out << toPlainText();
    textAnalizer.saveData(out);
}

void CustomTextEdit::readData(QDataStream &in, const QList<Style> &styles)
{
    QString data;
    in >> data;
    setPlainText(data);
    textAnalizer.readData(in, styles, textCursor());
}

void CustomTextEdit::searchSpecialText()
{
    QTextCursor cur = cursorForPosition(mapFromGlobal(QCursor::pos()));
    cur.select(QTextCursor::WordUnderCursor);

    if (textAnalizer.containsSpecialText(cur)) {
        QTextCharFormat charFormat = cur.charFormat();
        charFormat.setFontUnderline(true);
        cur.setCharFormat(charFormat);
        textAnalizer.specialTextSelectedCursor = cur;
    }
    else {
        textAnalizer.restoreOverridedCursor();
        textAnalizer.removeUnderlinedSpecialText();
    }
}

void CustomTextEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control) {
        searchBtnPressed = true;
    }
    else if (!event->text().isEmpty()) {
        QTextCursor cur = textCursor();
        if (cur.charFormat().background().color() != styles->at(0).getIconColor()) {
            QTextCharFormat charFormat = cur.charFormat();
            charFormat.setBackground(styles->at(0).getIconColor());
            cur.setCharFormat(charFormat);
            setTextCursor(cur);
        }
    }
    QTextEdit::keyPressEvent(event);
}

void CustomTextEdit::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control) {
        searchBtnPressed = false;
        textAnalizer.restoreOverridedCursor();
        textAnalizer.removeUnderlinedSpecialText();
    }
    QTextEdit::keyReleaseEvent(event);
}

void CustomTextEdit::mouseMoveEvent(QMouseEvent* event)
{
    if (searchBtnPressed) {
        searchSpecialText();
    }
    QTextEdit::mouseMoveEvent(event);
}

void CustomTextEdit::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!textAnalizer.specialTextSelectedCursor.selectedText().isEmpty()) {
            emit specialTextClicked();
        }
    }
    QTextEdit::mousePressEvent(event);
}

void CustomTextEdit::focusOutEvent(QFocusEvent *event)
{
    searchBtnPressed = false;
    textAnalizer.restoreOverridedCursor();
    textAnalizer.removeUnderlinedSpecialText();
    QTextEdit::focusOutEvent(event);
}

void CustomTextEdit::setTextSelectedColor()
{
    if (QAction* act = qobject_cast<QAction*>(sender())) {

        textAnalizer.setTextSelectedColor(textCursor(), *styles, act);
    }
}
