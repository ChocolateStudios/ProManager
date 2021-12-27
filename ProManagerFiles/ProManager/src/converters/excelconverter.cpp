#include "excelconverter.h"

ExcelConverter::ExcelConverter(MainWindow *mainWin)
    : BaseConverter(mainWin)
{

}

ExcelConverter::ExcelConverter(MainWindow *mainWin, const FileExportPropertiesResource &resource, const QString &exportDirectory)
    : BaseConverter(mainWin, resource, exportDirectory)
{

}

void ExcelConverter::exportTo()
{

}

void ExcelConverter::exportToPdf()
{

}

void ExcelConverter::import(const QString &filename, BaseEditor *editor)
{

}
