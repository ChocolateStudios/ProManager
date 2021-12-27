#ifndef POWERPOINTCONVERTER_H
#define POWERPOINTCONVERTER_H

#include "converters/baseconverter.h"

class PowerPointConverter : public BaseConverter
{
public:
    PowerPointConverter(MainWindow* mainWin);
    PowerPointConverter(MainWindow* mainWin, const FileExportPropertiesResource& resource, const QString& exportDirectory);

public:
    void exportTo() override;
    void exportToPdf() override;
    void import(const QString &filename, BaseEditor *editor) override;
};

#endif // POWERPOINTCONVERTER_H
