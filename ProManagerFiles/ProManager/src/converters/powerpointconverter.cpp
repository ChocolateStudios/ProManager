#include "powerpointconverter.h"

PowerPointConverter::PowerPointConverter(MainWindow *mainWin)
    : BaseConverter(mainWin)
{

}

PowerPointConverter::PowerPointConverter(MainWindow *mainWin, const FileExportPropertiesResource &resource, const QString &exportDirectory)
    : BaseConverter(mainWin, resource, exportDirectory)
{

}

void PowerPointConverter::exportTo()
{

}

void PowerPointConverter::exportToPdf()
{

}

void PowerPointConverter::import(const QString &filename, BaseEditor *editor)
{

}
