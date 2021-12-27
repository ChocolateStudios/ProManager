#ifndef SPREADSHEETTAGANALIZER_H
#define SPREADSHEETTAGANALIZER_H

#include "analizers/tag/taganalizer.h"

class BaseConverter;

class SpreadsheetTagAnalizer : public TagAnalizer
{
public:
    SpreadsheetTagAnalizer(BaseConverter* converter, ResourcesManager* resManager);

private:
    void doSomethingInWord(WordConverter* converter) override;
    void doSomethingInExcel(ExcelConverter* converter) override;
};

#endif // SPREADSHEETTAGANALIZER_H
