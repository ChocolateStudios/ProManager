#include "selectioninserter.h"

#include "word/font.h"
#include "word/picture.h"
#include "word/tableofcontents.h"
#include "word/table.h"

#include <QVariant>
#include <QAxObject>

namespace OfficeLib { namespace Word {

SelectionInserter::SelectionInserter(QAxObject *selectionObj, QAxObject* docObj)
    : Inserter(selectionObj, docObj)
{

}

void SelectionInserter::writeText(const QString &text)
{
    if (!inserterObj) return;
    inserterObj->dynamicCall("TypeText(const QString&)", text);
}

Font SelectionInserter::getFont()
{
    if (!inserterObj) return Font(nullptr);
    return Font(inserterObj->querySubObject("Font"));
}

void SelectionInserter::setParagraphAlignment(WdParagraphAlignment &alignment)
{
    if (!inserterObj) return;
    inserterObj->querySubObject("ParagraphFormat")->dynamicCall("SetAlignment(WdParagraphAlignment)", alignment);
    inserterObj->querySubObject("ParagraphFormat")->setProperty("Alignment", alignment);
}

QVariant SelectionInserter::getParagraphAlignment()
{
    if (!inserterObj) return QVariant();
    return inserterObj->querySubObject("ParagraphFormat")->dynamicCall("Alignment()");
}

void SelectionInserter::setHeader(const unsigned int &headerLevel)
{
    if (!inserterObj) return;
    if (headerLevel <= 0)
        inserterObj->setProperty("Style", "Normal");
    else
        inserterObj->setProperty("Style", QString("Heading %1").arg(headerLevel));
}

Picture SelectionInserter::insertPicture(const QString &filename)
{
    if (!inserterObj) return Picture(nullptr);
    Picture picture = Picture(docObj->querySubObject("InlineShapes()")->querySubObject("AddPicture(QString&)", filename));
    inserterObj->dynamicCall("EndKey()");
    return picture;
}

TableOfContents SelectionInserter::insertTableOfContents()
{
    if (!inserterObj) return TableOfContents(nullptr);
    QAxObject* range = inserterObj->querySubObject("Range");
    QAxObject* tablesOfContents = docObj->querySubObject("TablesOfContents");

    return TableOfContents(tablesOfContents->querySubObject("Add(Range*)", range->asVariant()));
}

Table SelectionInserter::insertTable(int rows, int columns)
{
    if (!inserterObj) return Table(nullptr, -1, -1);
    QAxObject* range = inserterObj->querySubObject("Range");
    QAxObject* tables = inserterObj->querySubObject("Tables");

    Table table = Table(tables->querySubObject("Add(Range*, int, int)", range->asVariant(), rows, columns), rows, columns);
    table.setTableStyle("Table Grid");

    return table;
}

}}
