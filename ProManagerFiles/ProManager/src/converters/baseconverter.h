#ifndef BASECONVERTER_H
#define BASECONVERTER_H

#include "resources/fileexportpropertiesresource.h"

#include <QString>

class FileExportPropertiesResource;
class BaseEditor;
class MainWindow;

class BaseConverter
{
public:
    BaseConverter(MainWindow* mainWin)
        : mainWin(mainWin)
    {
    }

    BaseConverter(MainWindow* mainWin, const FileExportPropertiesResource& resource, const QString& exportDirectory)
        : mainWin(mainWin), resource(resource), exportDirectory(exportDirectory)
    {
    }

    virtual ~BaseConverter()
    {
    }

    virtual void exportTo() = 0;
    virtual void exportToPdf() = 0;
    virtual void import(const QString &filename, BaseEditor *editor) = 0;

protected:
    MainWindow* mainWin;
    FileExportPropertiesResource resource;
    QString exportDirectory;
};

#endif // BASECONVERTER_H
