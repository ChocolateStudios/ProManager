#ifndef SPREADSHEETEDITOR_H
#define SPREADSHEETEDITOR_H

#include "editors/baseeditor.h"

#include <QWidget>

class SpreadSheet;

class SpreadsheetEditor : public QWidget, public BaseEditor
{
public:
    SpreadsheetEditor();

    friend QDataStream &operator<<(QDataStream& out, SpreadsheetEditor* editor);
    friend QDataStream &operator>>(QDataStream& in, SpreadsheetEditor* editor);

    void insertColumnRightInTable() override;
    void insertColumnLeftInTable() override;
    void insertRowDownInTable() override;
    void insertRowUpInTable() override;
    void removeCurrentColumnInTable() override;
    void removeCurrentRowInTable() override;

private:
    SpreadSheet* spreadsheet;
};

#endif // SPREADSHEETEDITOR_H
