#ifndef EXCELCONVERTER_H
#define EXCELCONVERTER_H

#include "converters/baseconverter.h"

class ExcelConverter : public BaseConverter
{
public:
    ExcelConverter(MainWindow* mainWin);
    ExcelConverter(MainWindow* mainWin, const FileExportPropertiesResource& resource, const QString& exportDirectory);

    void exportTo() override;
    void exportToPdf() override;
    void import(const QString &filename, BaseEditor *editor) override;
};

#endif // EXCELCONVERTER_H
