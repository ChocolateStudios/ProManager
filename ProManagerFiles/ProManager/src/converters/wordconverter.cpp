#include "wordconverter.h"

#include "mainwindow.h"
#include "maintabwidget.h"

#include "resources/fileexportpropertiesresource.h"
#include "resources/styleresource.h"
#include "resources/manager/resourcesmanager.h"

#include "analizers/tag/textdocumenttaganalizer.h"

#include "editors/textdocumenteditor.h"

#include "word/word.h"
#include "word/table.h"
#include "word/font.h"
#include "word/tableofcontents.h"
#include "word/rangeinserter.h"

WordConverter::WordConverter(MainWindow* mainWin)
    : BaseConverter(mainWin)
{
}

WordConverter::WordConverter(MainWindow *mainWin, const FileExportPropertiesResource &resource, const QString &exportDirectory)
    : BaseConverter(mainWin, resource, exportDirectory)
{
}

WordConverter::~WordConverter()
{
    if (fileOpened)
        closeDocument();
}

void WordConverter::exportTo()
{
    createDocument();
    fillDocument();
    const QString& saveAsName = QString("%1/%2%3").arg(exportDirectory, resource.getName(), resource.getExtension());
    saveDocumentAs(saveAsName);
}

void WordConverter::exportToPdf()
{
    const QString& saveAsName = QString("%1/%2%3").arg(exportDirectory, resource.getName(), ".pdf");
    if (fileOpened) {
        saveDocumentAs(saveAsName);
    }
    else {
        createDocument();
        fillDocument();
        saveDocumentAs(saveAsName);
    }
}

void WordConverter::import(const QString &filename, BaseEditor *editor)
{
    qDebug() << filename << editor;
}

void WordConverter::fillDocumentWithEditorName(const QString &editorName, OfficeLib::Word::Inserter* _inserter, QList<OfficeLib::Word::TableOfContents>& tablesToUpdate)
{
    BaseEditor* editor = mainWin->mainTabWidget->getEditorByName(editorName);

    OfficeLib::Word::Inserter* inserter = _inserter ? _inserter : document->getSelectionInserter();

    if (TextDocumentEditor* textDocEditor = dynamic_cast<TextDocumentEditor*>(editor)) {
        fillDocumentFromTextDocumentEditor(textDocEditor, *inserter, tablesToUpdate);
    }
    else if (SpreadsheetEditor* spreadsheetEditor = dynamic_cast<SpreadsheetEditor*>(editor)) {
        fillDocumentFromSpreadsheetEditor(spreadsheetEditor, *inserter);
    }
}

inline void WordConverter::createDocument()
{
    word = new OfficeLib::Word::Word();
//    word->setVisible(true);
    const QString& templateFile = resource.getTemplateFile();

    document = templateFile.isEmpty() ? word->createDocument() :
        word->openDocument(mainWin->resourcesManager->getResourcesController<FileResource>(2)->
                           getResourceByName(templateFile)->getPath());

    fileOpened = true;
    document->moveEndSelection(OfficeLib::Word::WdUnits::wdStory, OfficeLib::Word::WdMovementType::wdMove);
}

void WordConverter::saveDocumentAs(const QString &targetFileName)
{
    document->saveAs(targetFileName);
}

void WordConverter::closeDocument()
{
    document->close();
    word->quit();

    fileOpened = false;

    delete document;
    delete word;
}

inline void WordConverter::fillDocument()
{
    const QStringList& sections = resource.getSectionsBlockChain();
    int size = sections.size();


//    OfficeLib::Word::Inserter* selectionInserter = document->getSelectionInserter();
    OfficeLib::Word::Inserter* selectionInserter = document->getSelectionRangeInserter();

    QList<OfficeLib::Word::TableOfContents> tablesToUpdate;

    for (int i = 0; i < size; i++) {
        BaseEditor* editor = mainWin->mainTabWidget->getEditorByName(sections[i]);

        if (TextDocumentEditor* textDocEditor = dynamic_cast<TextDocumentEditor*>(editor)) {
            fillDocumentFromTextDocumentEditor(textDocEditor, *selectionInserter, tablesToUpdate);
        }
        else if (SpreadsheetEditor* spreadsheetEditor = dynamic_cast<SpreadsheetEditor*>(editor)) {
            fillDocumentFromSpreadsheetEditor(spreadsheetEditor, *selectionInserter);
        }
    }

    qDebug() << "Updating";

    for (OfficeLib::Word::TableOfContents& t : tablesToUpdate) {
        t.update();
    }
}

void WordConverter::fillDocumentFromTextDocumentEditor(TextDocumentEditor* editor, OfficeLib::Word::Inserter& inserter, QList<OfficeLib::Word::TableOfContents>& tablesToUpdate)
{
    TagAnalizer* analizer = new TextDocumentTagAnalizer(this, mainWin->resourcesManager, &inserter, tablesToUpdate);

    QTextCursor mainCursor(editor->textCursor());
    mainCursor.movePosition(QTextCursor::End);
    int finalPos = mainCursor.position();
    mainCursor.movePosition(QTextCursor::Start);

    if (mainCursor.position() == finalPos) {
        delete analizer;
        return;
    }

    QColor col;

    do {
        if (mainCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor)) {
            col = mainCursor.charFormat().background().color();

            QChar t = mainCursor.selectedText().at(0);
            if (t == QChar(8233)) {       //Check for enter character (code 8233)
               document->writeText(inserter, "\n");
//               mainCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
//               continue;
            }

            const StyleResource* curStyle = ((StyleResourcesController*)mainWin->resourcesManager->getResourcesController<StyleResource>(0))->getStyleByIconColor(col);

            if (curStyle) {
                int lastEndCursorPosition = 0;
                while (mainCursor.charFormat().background().color() == col) {
                    processTag(mainCursor, curStyle, inserter, analizer, lastEndCursorPosition);
                    processTable(mainCursor, curStyle, inserter, lastEndCursorPosition);

                    lastEndCursorPosition = mainCursor.selectionEnd();
                    if (!mainCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor))
                        break;
                }

                writeText(mainCursor, curStyle, inserter);
            }
        }
        mainCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
    } while (mainCursor.position() < finalPos);

    delete analizer;
}

void WordConverter::fillDocumentFromSpreadsheetEditor(SpreadsheetEditor* editor, OfficeLib::Word::Inserter& inserter)
{

}

void WordConverter::setTextProperties(const StyleResource &style, OfficeLib::Word::Inserter& inserter)
{
    //Font Properties
    QFont font = style.getFont();
    OfficeLib::Word::WdColor fontColor = style.getFontColor();
    unsigned short headerLevel = style.getHeaderLevel();

    document->setHeader(inserter, headerLevel);
    OfficeLib::Word::Font selFont = document->getInserterFont(inserter);
    selFont.setFontColor(fontColor);
    selFont.setFontBold(font.bold());
    selFont.setFontName(font.family());
    selFont.setFontSize(font.pointSize());

    //Paragraph Properties
    OfficeLib::Word::WdParagraphAlignment alignment = OfficeLib::Word::wdAlignParagraphJustify;
    document->setParagraphAlignment(inserter, alignment);
}

void WordConverter::writeText(QTextCursor& mainCursor, const StyleResource* curStyle, OfficeLib::Word::Inserter& inserter)
{
    QString text = mainCursor.selectedText();
    if (!text.isEmpty()) {
        text.replace(QChar(8233), "\n");

        if (text.at(0) == '\n') {
            if (text.size() > 1)
                text.remove(0, 1);
            else
                text = "";
        }

        if (!text.isEmpty()) {
            OfficeLib::Word::Inserter* inserterPtr = &inserter;
            if (OfficeLib::Word::RangeInserter* rangeInserter = dynamic_cast<OfficeLib::Word::RangeInserter*>(inserterPtr)){
                document->writeText(inserter, text);
                setTextProperties(*curStyle, inserter);
                rangeInserter->afterWriteText();
            }
            else {
                setTextProperties(*curStyle, inserter);
                document->writeText(inserter, text);
            }
        }
    }
}

void WordConverter::processTag(QTextCursor& mainCursor, const StyleResource* curStyle, OfficeLib::Word::Inserter& inserter, TagAnalizer* analizer, int& lastEndCursorPosition)
{
    if (mainCursor.selectedText().endsWith("<")) {
        mainCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

        QTextCursor specialTextCursor(mainCursor);
        specialTextCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
        specialTextCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

        if (TagAnalizer::containsTag(specialTextCursor)) {
            writeText(mainCursor, curStyle, inserter);

            analizer->doSomething(specialTextCursor.selectedText());

            mainCursor = QTextCursor(specialTextCursor);
            mainCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
        }
        else {
            mainCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            lastEndCursorPosition = mainCursor.selectionEnd();
            mainCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        }
    }
}

void WordConverter::processTable(QTextCursor& mainCursor, const StyleResource* curStyle, OfficeLib::Word::Inserter& inserter, int& lastEndCursorPosition)
{
    QTextCursor tableCursor(mainCursor);

    int selecEnd = mainCursor.selectionEnd();
    if (mainCursor.hasSelection() && selecEnd - lastEndCursorPosition > 2) {
        QTextTable* textTable = nullptr;
        for (int i = lastEndCursorPosition + 1; i < selecEnd - 1; i++) {
            tableCursor.setPosition(i, QTextCursor::MoveAnchor);
            if (textTable = tableCursor.currentTable())
                break;
        }

        if (textTable) {
            mainCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
            writeText(mainCursor, curStyle, inserter);

            OfficeLib::Word::Table table = document->insertTable(inserter, textTable->rows(), textTable->columns());

            for (int i = 0; i < textTable->rows(); i++) {
                for (int j = 0; j < textTable->columns(); j++) {
                    tableCursor.setPosition(textTable->cellAt(i, j).firstPosition(), QTextCursor::MoveAnchor);
                    tableCursor.setPosition(textTable->cellAt(i, j).lastPosition(), QTextCursor::KeepAnchor);

                    QString text = tableCursor.selectedText();
                    if (!text.isEmpty()) {
                        text.replace(QChar(8233), "\n");

                        QFont font = curStyle->getFont();
                        OfficeLib::Word::WdColor fontColor = curStyle->getFontColor();

                        table.insertTextAtCell(i + 1, j + 1, text);
                        OfficeLib::Word::Font tableFont = table.getFontAtCell(i + 1, j + 1);
                        tableFont.setFontColor(fontColor);
                        tableFont.setFontBold(font.bold());
                        tableFont.setFontName(font.family());
                        tableFont.setFontSize(font.pointSize());

                    }
                }
            }

            mainCursor.setPosition(selecEnd, QTextCursor::MoveAnchor);
            document->moveEndSelection(OfficeLib::Word::WdUnits::wdStory, OfficeLib::Word::WdMovementType::wdMove);
        }
    }
}
