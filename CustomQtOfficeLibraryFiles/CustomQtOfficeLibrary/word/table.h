#ifndef TABLE_H
#define TABLE_H

#include "wordenums.h"
#include "font.h"

class QAxObject;
class QString;
class QVariant;

namespace OfficeLib { namespace Word {
class RangeInserter;

class Table
{
public:
    Table(QAxObject* tblObj, int rows, int columns);
    Table(QAxObject* tblObj, RangeInserter* inserter, int rows, int columns);
    void insertTextAtCell(int row, int column, const QString& text);
    void setTableStyle(const QString& style);
    int getLastCellEndRange();

    Font getFontAtCell(int row, int column);

private:
    QAxObject* tableObj;
    RangeInserter* rangeInserter;
    int rows, columns;
};

}}

#endif // TABLE_H
