#ifndef TEXTDOCUMENTEDITOR_H
#define TEXTDOCUMENTEDITOR_H

#include "editors/baseeditor.h"
#include "textrange/textrange.h"
#include "textrange/textrangescontroller.h"

#include <QTextTable>
#include <QTextEdit>

class StyleResourcesController;

class TextDocumentEditor : public QTextEdit, public BaseEditor
{
    Q_OBJECT
public:
    TextDocumentEditor(StyleResourcesController* styleResController);

    friend QDataStream &operator<<(QDataStream& out, TextDocumentEditor* editor);
    friend QDataStream &operator>>(QDataStream& in, TextDocumentEditor* editor);

    void insertTable();
    void insertColumnRightInTable() override;
    void insertColumnLeftInTable() override;
    void insertRowDownInTable() override;
    void insertRowUpInTable() override;
    void removeTable();
    void removeCurrentColumnInTable() override;
    void removeCurrentRowInTable() override;

    void showCharactersIndex();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void insertFromMimeData(const QMimeData* source) override;

signals:
    void mouseMoved();
    void leftMouseBtnClicked();
    void tagSearcherBtnReleasedText();
    void colorSelectedText(const StyleResource& newStyle);

public slots:
    void updateTextRanges();

private slots:
    void onTextChanged();

public:
    QList<QTextTable*> textTables;
    TextRangesController textRangesController;

private:
    bool searchBtnPressed = false;

    unsigned short lastTextLength = 0;
    StyleResourcesController* styleResourcesController;
};

#endif // TEXTDOCUMENTEDITOR_H
