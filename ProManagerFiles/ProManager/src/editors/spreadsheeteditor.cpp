#include "spreadsheeteditor.h"

#include "spreadsheet/spreadsheet.h"

#include <QVBoxLayout>

SpreadsheetEditor::SpreadsheetEditor()
{
    spreadsheet = new SpreadSheet(10, 6);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(spreadsheet);

    setLayout(mainLayout);
}

void SpreadsheetEditor::insertColumnRightInTable()
{

}

void SpreadsheetEditor::insertColumnLeftInTable()
{

}

void SpreadsheetEditor::insertRowDownInTable()
{

}

void SpreadsheetEditor::insertRowUpInTable()
{

}

void SpreadsheetEditor::removeCurrentColumnInTable()
{

}

void SpreadsheetEditor::removeCurrentRowInTable()
{

}


QDataStream &operator<<(QDataStream& out, SpreadsheetEditor* editor){
    return out;
}


QDataStream &operator>>(QDataStream& in, SpreadsheetEditor* editor){
    return in;
}
