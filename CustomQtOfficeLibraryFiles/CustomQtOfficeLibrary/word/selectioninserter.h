#ifndef SELECTIONINSERTER_H
#define SELECTIONINSERTER_H

#include "word/inserter.h"

namespace OfficeLib { namespace Word {

class SelectionInserter : public Inserter
{
public:
    SelectionInserter(QAxObject* selectionObj, QAxObject* docObj);

    void writeText(const QString &text) override;
    Font getFont() override;
    void setParagraphAlignment(WdParagraphAlignment &alignment) override;
    QVariant getParagraphAlignment() override;
    void setHeader(const unsigned int &headerLevel) override;
    Picture insertPicture(const QString &filename) override;
    TableOfContents insertTableOfContents() override;
    Table insertTable(int rows, int columns) override;
};

}}

#endif // SELECTIONINSERTER_H
