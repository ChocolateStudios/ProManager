#include "spreadsheet.h"

#include "spreadsheet/spreadsheetdelegate.h"

SpreadSheet::SpreadSheet(int rows, int columns)
    : QTableWidget(rows, columns)
{
    setSizeAdjustPolicy(QTableWidget::AdjustToContents);

    for (int c = 0; c < 10; ++c) {
        QString character(QChar('A' + c));
        setHorizontalHeaderItem(c, new QTableWidgetItem(character));
    }

    setItemPrototype(item(rows - 1, columns - 1));
    setItemDelegate(new SpreadSheetDelegate);
}
