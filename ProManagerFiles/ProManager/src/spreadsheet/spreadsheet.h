#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QTableWidget>

class SpreadSheet : public QTableWidget
{
public:
    SpreadSheet(int rows, int columns);
};

#endif // SPREADSHEET_H
