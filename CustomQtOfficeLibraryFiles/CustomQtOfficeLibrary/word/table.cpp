#include "table.h"

#include "rangeinserter.h"

#include <QAxObject>

namespace OfficeLib { namespace Word {

Table::Table(QAxObject *tblObj, int rows, int columns)
    : tableObj(tblObj), rows(rows), columns(columns)
{
}

Table::Table(QAxObject *tblObj, RangeInserter *rangeInserter, int rows, int columns)
    : tableObj(tblObj), rangeInserter(rangeInserter), rows(rows), columns(columns)
{

}

void Table::insertTextAtCell(int row, int column, const QString &text)
{
    if (!tableObj) return;
    QAxObject* cell = tableObj->querySubObject("Cell(int, int)", row, column);
    cell->querySubObject("Range")->dynamicCall("SetText(const QString&)", text);

    if (rangeInserter) {
        rangeInserter->setStartRange(getLastCellEndRange() + 1);
    }
}

void Table::setTableStyle(const QString &style)
{
    if (!tableObj) return;
    tableObj->setProperty("Style", style);
}

int Table::getLastCellEndRange()
{
    QAxObject* cellRange = tableObj->querySubObject("Cell(int, int)", rows, columns)->querySubObject("Range");
    return cellRange->dynamicCall("End").toInt();
}

Font Table::getFontAtCell(int row, int column)
{
    if (!tableObj) return Font(nullptr);
    QAxObject* cell = tableObj->querySubObject("Cell(int, int)", row, column);
    QAxObject* range = cell->querySubObject("Range");
    return Font(range->querySubObject("Font"));
}

}}
