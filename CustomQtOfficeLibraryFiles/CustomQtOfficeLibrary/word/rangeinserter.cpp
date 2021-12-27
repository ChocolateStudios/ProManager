#include "rangeinserter.h"

#include "word/font.h"
#include "word/picture.h"
#include "word/tableofcontents.h"
#include "word/table.h"

#include <QVariant>
#include <QAxObject>

namespace OfficeLib { namespace Word {

RangeInserter::RangeInserter(QAxObject *rangeObj, QAxObject* docObj)
    : Inserter(rangeObj, docObj)
{

}

void RangeInserter::afterWriteText()
{
    setStartRange(getEndRange());
}

void RangeInserter::writeText(const QString &text)
{
    if (!inserterObj) return;
    inserterObj->dynamicCall("SetText(const QString&)", text);
}

Font RangeInserter::getFont()
{
    if (!inserterObj) return Font(nullptr);
    return Font(inserterObj->querySubObject("Font"));
}

void RangeInserter::setParagraphAlignment(WdParagraphAlignment &alignment)
{
    if (!inserterObj) return;
    inserterObj->querySubObject("ParagraphFormat")->dynamicCall("SetAlignment(WdParagraphAlignment)", alignment);
    inserterObj->querySubObject("ParagraphFormat")->setProperty("Alignment", alignment);
}

QVariant RangeInserter::getParagraphAlignment()
{
    if (!inserterObj) return QVariant();
    return inserterObj->querySubObject("ParagraphFormat")->dynamicCall("Alignment()");
}

void RangeInserter::setHeader(const unsigned int &headerLevel)
{
    if (!inserterObj) return;
    if (headerLevel <= 0)
        inserterObj->setProperty("Style", "Normal");
    else
        inserterObj->setProperty("Style", QString("Heading %1").arg(headerLevel));
}

Picture RangeInserter::insertPicture(const QString &filename)
{
    if (!inserterObj) return Picture(nullptr);
    Picture picture = Picture(inserterObj->querySubObject("InlineShapes()")->querySubObject("AddPicture(QString&)", filename));
    setStartRange(getEndRange() + 1);
    return picture;
}

TableOfContents RangeInserter::insertTableOfContents()
{
    if (!inserterObj) return TableOfContents(nullptr);
//    QAxObject* range = selection->querySubObject("Range");
    QAxObject* tablesOfContents = docObj->querySubObject("TablesOfContents");

    return TableOfContents(tablesOfContents->querySubObject("Add(Range*)", inserterObj->asVariant()));
}

Table RangeInserter::insertTable(int rows, int columns)
{
    if (!inserterObj) return Table(nullptr, -1, -1);
    QAxObject* tables = inserterObj->querySubObject("Tables");

    Table table = Table(tables->querySubObject("Add(Range*, int, int)", inserterObj->asVariant(), rows, columns), this, rows, columns);
    table.setTableStyle("Table Grid");
    setStartRange(table.getLastCellEndRange() + 1);

    return table;
}

int RangeInserter::getStartRange()
{
    if (!inserterObj) return -1;
    return inserterObj->dynamicCall("Start()").toInt();
}

int RangeInserter::getEndRange()
{
    if (!inserterObj) return -1;
    return inserterObj->dynamicCall("End()").toInt();
}

void RangeInserter::setStartRange(int start)
{
    if (!inserterObj) return;
    inserterObj->setProperty("Start", start);
}

}}
