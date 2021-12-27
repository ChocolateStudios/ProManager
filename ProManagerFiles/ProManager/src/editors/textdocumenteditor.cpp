#include "textdocumenteditor.h"

#include "resources/controllers/styleresourcescontroller.h"

#include <QTextTable>

TextDocumentEditor::TextDocumentEditor(StyleResourcesController *styleResController)
    : textRangesController(styleResController), styleResourcesController(styleResController)
{
    connect(this, &QTextEdit::textChanged, this, &TextDocumentEditor::onTextChanged);
    connect(this, &QTextEdit::cursorPositionChanged, this, &TextDocumentEditor::updateTextRanges);

    //Set default background color
    QTextCursor cur(textCursor());
    QTextCharFormat charFormat = cur.charFormat();
    charFormat.setBackground(styleResourcesController->getResource(0).getIconColor());
    cur.setCharFormat(charFormat);
    setTextCursor(cur);
}

void TextDocumentEditor::insertTable()
{
    QTextCursor cur = textCursor();
    QTextTable* table = cur.currentTable();
    if (table) return;

    table = textCursor().insertTable(1, 1);
    textTables.append(table);

    connect(table, &QTextTable::destroyed, this,[&](QObject* obj) {
        if (obj) {
            textTables.removeOne(obj);
        }
    });
}

void TextDocumentEditor::insertColumnRightInTable()
{
    QTextCursor cur = textCursor();
    QTextTable* table = cur.currentTable();
    if (table) {
        int currentColumn = table->cellAt(cur).column();
        table->insertColumns(currentColumn + 1, 1);
    }
}

void TextDocumentEditor::insertColumnLeftInTable()
{
    QTextCursor cur = textCursor();
    QTextTable* table = cur.currentTable();
    if (table) {
        int currentColumn = table->cellAt(cur).column();
        table->insertColumns(currentColumn, 1);
    }
}

void TextDocumentEditor::insertRowDownInTable()
{
    QTextCursor cur = textCursor();
    QTextTable* table = cur.currentTable();
    if (table) {
        int currentRow = table->cellAt(cur).row();
        table->insertRows(currentRow + 1, 1);
    }
}

void TextDocumentEditor::insertRowUpInTable()
{
    QTextCursor cur = textCursor();
    QTextTable* table = cur.currentTable();
    if (table) {
        int currentRow = table->cellAt(cur).row();
        table->insertRows(currentRow, 1);
    }
}

void TextDocumentEditor::removeTable()
{
    QTextCursor cur = textCursor();
    QTextTable* table = cur.currentTable();
    if (table) {
        table->removeRows(0, table->rows());
    }
}

void TextDocumentEditor::removeCurrentColumnInTable()
{
    QTextCursor cur = textCursor();
    QTextTable* table = cur.currentTable();
    if (table) {
        int currentColumn = table->cellAt(cur).column();
        table->removeColumns(currentColumn, 1);
    }
}

void TextDocumentEditor::removeCurrentRowInTable()
{
    QTextCursor cur = textCursor();
    QTextTable* table = cur.currentTable();
    if (table) {
        int currentRow = table->cellAt(cur).row();
        table->removeRows(currentRow, 1);
    }
}

void TextDocumentEditor::showCharactersIndex()
{
    QTextCursor cur(textCursor());
    QString debugText = QString("[%1, %2] - %3")
            .arg(cur.selectionStart()).arg(cur.selectionEnd()).arg(cur.selectionEnd() - cur.selectionStart());
    qDebug() << debugText;

//    cur.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);

//    int i = 0;

//    cur.beginEditBlock();
//    do {
//        cur.insertText(QString("%1").arg(i));
//        i++;
//    } while (cur.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor));
//    cur.endEditBlock();
}

void TextDocumentEditor::keyPressEvent(QKeyEvent *event)
{
    if (!event->text().isEmpty()) {
        QTextCursor cur = textCursor();

        if (event->key() == Qt::Key_Tab && cur.currentTable()) {
            QTextTable* table = cur.currentTable();
            int currentColumn = table->cellAt(cur).column();
            table->insertColumns(currentColumn + 1, 1);
            return;
        }
    }
    else if (event->key() == Qt::Key_Control) {
        searchBtnPressed = true;
    }

    QTextEdit::keyPressEvent(event);
}

void TextDocumentEditor::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) {
        searchBtnPressed = false;
        emit tagSearcherBtnReleasedText();
    }
    QTextEdit::keyReleaseEvent(event);
}

void TextDocumentEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (searchBtnPressed) {
        emit mouseMoved();
    }
    QTextEdit::mouseMoveEvent(event);
}

void TextDocumentEditor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (searchBtnPressed) {
            emit leftMouseBtnClicked();
        }
    }
    QTextEdit::mousePressEvent(event);
}

void TextDocumentEditor::focusOutEvent(QFocusEvent *event)
{
    searchBtnPressed = false;
    emit tagSearcherBtnReleasedText();
    QTextEdit::focusOutEvent(event);
}

void TextDocumentEditor::insertFromMimeData(const QMimeData *source)
{
    int start = textCursor().selectionStart();

    QTextEdit::insertFromMimeData(source);

    QTextCursor cur(textCursor());
    int end = cur.selectionEnd();

    cur.setPosition(start, QTextCursor::MoveAnchor);
    cur.setPosition(end, QTextCursor::KeepAnchor);

    QTextCharFormat newCharFormat = cur.charFormat();
    newCharFormat.setForeground(QBrush(Qt::black));

    QFont newFont = newCharFormat.font();
    newFont.setPointSize(9);
    newFont.setBold(false);
    newFont.setItalic(false);
    newFont.setFamily("Solano Gothic MVB  Light");
    newFont.setUnderline(false);

    newCharFormat.setFont(newFont);

    cur.setCharFormat(newCharFormat);

    if (StyleResource* cStyle = styleResourcesController->getStyleByIconColor(newCharFormat.background().color())) {
        setTextCursor(cur);
        emit colorSelectedText(*cStyle);
    }

    cur.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
    setTextCursor(cur);
}

void TextDocumentEditor::onTextChanged()
{
    int newLength = toPlainText().length();
    int countAddToRanges = newLength - lastTextLength;
    lastTextLength = newLength;

    QTextCursor cur(textCursor());
    textRangesController.addCountToAdd(countAddToRanges, cur);
    textRangesController.setStartPosition(cur.position() - countAddToRanges);

    if (countAddToRanges < 0) {
        textRangesController.removeQuantityToStyleRanges(cur);
    }
}

void TextDocumentEditor::updateTextRanges()
{
    int newLength = toPlainText().length();
    int textLengthCompare = newLength - lastTextLength;

    if (textLengthCompare == 0) {
        QTextCursor cur(textCursor());
        textRangesController.addQuantityToStyleRanges(cur);
    }
}



QDataStream &operator>>(QDataStream& in, TextDocumentEditor* editor){
    QString text;
    in >> text;

    QTextCursor cur(editor->textCursor());
    cur.insertText(text);

    in >> editor->textRangesController;

    int size;
    in >> size;

    for (int i = 0; i < size; i++) {
        int first, last, rows, columns;
        in >> first >> last >> rows >> columns;

        cur.setPosition(first, QTextCursor::MoveAnchor);
        cur.setPosition(last, QTextCursor::KeepAnchor);

        QString text = cur.selectedText();
        QStringList texts = text.split(QChar(8233));

        cur.setPosition(first - 1, QTextCursor::MoveAnchor);
        cur.setPosition(last + 1, QTextCursor::KeepAnchor);
        cur.removeSelectedText();

        QTextTable* table = cur.insertTable(rows, columns);
        int rowsCount = table->rows();
        int columnCount = table->columns();
        for (int j = 0; j < rowsCount; j++) {
            for (int w = 0; w < columnCount; w++) {
                table->cellAt(j, w).firstCursorPosition().insertText(texts[j * columnCount + w]);
            }
        }
        editor->textTables.append(table);
    }
    return in;
}

QDataStream &operator<<(QDataStream& out, TextDocumentEditor* editor){
    editor->updateTextRanges();

    QTextCursor cur(editor->textCursor());
    cur.setPosition(0, QTextCursor::MoveAnchor);
    cur.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    const QString& text = cur.selectedText();

    out << text;

    out << editor->textRangesController;

    QList<QTextTable*>& textTables = editor->textTables;
    int size = textTables.size();
    out << size;

    for (int i = 0; i < size; i++) {
        out << textTables[i]->firstPosition() << textTables[i]->lastPosition()
            << textTables[i]->rows() << textTables[i]->columns();
    }

    return out;
}
