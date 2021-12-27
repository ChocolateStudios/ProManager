#ifndef TEXTDOCUMENTTAGANALIZER_H
#define TEXTDOCUMENTTAGANALIZER_H

#include "analizers/tag/taganalizer.h"

namespace OfficeLib { namespace Word {
class Document;
class Inserter;
class TableOfContents;
}}

class TextDocumentTagAnalizer : public TagAnalizer
{
public:
    TextDocumentTagAnalizer(BaseConverter* converter, ResourcesManager* resManager, OfficeLib::Word::Inserter* inserter, QList<OfficeLib::Word::TableOfContents>& tablesToUpdate);

private:
    void doSomethingInWord(WordConverter* converter) override;
    void doSomethingInExcel(ExcelConverter* converter) override;

    //Word Tags
    void imgTag(OfficeLib::Word::Document* wordDocument);
    void pageBreakTag(OfficeLib::Word::Document* wordDocument);
    void fileTag(OfficeLib::Word::Document* wordDocument, WordConverter* converter);
    void tableOfContentTag(OfficeLib::Word::Document* wordDocument);

private:
    OfficeLib::Word::Inserter* inserter;
    QList<OfficeLib::Word::TableOfContents>& tablesToUpdate;
};

#endif // TEXTDOCUMENTTAGANALIZER_H
