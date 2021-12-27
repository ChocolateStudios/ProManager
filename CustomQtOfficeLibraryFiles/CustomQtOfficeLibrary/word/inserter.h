#ifndef INSERTER_H
#define INSERTER_H

#include "word/wordenums.h"

class QAxObject;
class QString;
class QVariant;

namespace OfficeLib { namespace Word {

class Font;
class Picture;
class TableOfContents;
class Table;

class Inserter
{
public:
    Inserter(QAxObject* inserterObj, QAxObject* docObj)
        : inserterObj(inserterObj), docObj(docObj)
    {
    }

    virtual void writeText(const QString& text) = 0;

    virtual Font getFont() = 0;

    virtual void setParagraphAlignment(WdParagraphAlignment& alignment) = 0;
    virtual QVariant getParagraphAlignment() = 0;
    virtual void setHeader(const unsigned int& headerLevel) = 0;

    virtual Picture insertPicture(const QString& filename) = 0;
    virtual TableOfContents insertTableOfContents() = 0;
    virtual Table insertTable(int rows = 1, int columns = 1) = 0;

protected:
    QAxObject* inserterObj;
    QAxObject* docObj;
};

}}

#endif // INSERTER_H
