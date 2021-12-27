#ifndef WORDCONVERTER_H
#define WORDCONVERTER_H

#include "converters/baseconverter.h"

class StyleResource;
class TextDocumentTagAnalizer;
class QTextCursor;
class TextDocumentEditor;
class SpreadsheetEditor;
class TagAnalizer;

namespace OfficeLib { namespace Word {
    class Word;
    class Document;
    class Inserter;
    class TableOfContents;
}}

class WordConverter : public BaseConverter
{
public:
    WordConverter(MainWindow* mainWin);
    WordConverter(MainWindow* mainWin, const FileExportPropertiesResource& resource, const QString& exportDirectory);
    ~WordConverter();

    void exportTo() override;
    void exportToPdf() override;
    void import(const QString &filename, BaseEditor *editor) override;

    //Document
    void fillDocumentWithEditorName(const QString& editorName, OfficeLib::Word::Inserter* inserter, QList<OfficeLib::Word::TableOfContents>& tablesToUpdate);

private:
    //Document
    void createDocument();
    void saveDocumentAs(const QString& targetFileName);
    void closeDocument();

    void fillDocument();
    void fillDocumentFromTextDocumentEditor(TextDocumentEditor* editor, OfficeLib::Word::Inserter& inserter, QList<OfficeLib::Word::TableOfContents>& tablesToUpdate);
    void fillDocumentFromSpreadsheetEditor(SpreadsheetEditor* editor, OfficeLib::Word::Inserter& inserter);

    //Text
    void setTextProperties(const StyleResource& style, OfficeLib::Word::Inserter& inserter);
    void writeText(QTextCursor& mainCursor, const StyleResource* curStyle, OfficeLib::Word::Inserter& inserter);

    //Process
    inline void processTag(QTextCursor& mainCursor, const StyleResource* curStyle, OfficeLib::Word::Inserter& inserter, TagAnalizer* analizer, int& lastEndCursorPosition);
    inline void processTable(QTextCursor& mainCursor, const StyleResource* curStyle, OfficeLib::Word::Inserter& inserter, int& lastEndCursorPosition);

    OfficeLib::Word::Word* word;
    bool fileOpened = false;

public:
    OfficeLib::Word::Document* document;
};

#endif // WORDCONVERTER_H
