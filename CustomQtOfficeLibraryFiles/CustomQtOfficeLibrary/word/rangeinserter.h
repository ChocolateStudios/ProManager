#ifndef RANGEINSERTER_H
#define RANGEINSERTER_H

#include "word/inserter.h"

namespace OfficeLib { namespace Word {

class RangeInserter : public Inserter
{
public:
    RangeInserter(QAxObject* rangeObj, QAxObject* docObj);

    void afterWriteText();
    void writeText(const QString &text) override;
    Font getFont() override;
    void setParagraphAlignment(WdParagraphAlignment &alignment) override;
    QVariant getParagraphAlignment() override;
    void setHeader(const unsigned int &headerLevel) override;
    Picture insertPicture(const QString &filename) override;
    TableOfContents insertTableOfContents() override;
    Table insertTable(int rows, int columns) override;

    int getStartRange();
    int getEndRange();
    void setStartRange(int start);
};

}}

#endif // RANGEINSERTER_H
