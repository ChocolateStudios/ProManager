#include "exporter.h"

#include "mainwindow.h"
#include "maintabwidget.h"

#include "resources/manager/resourcesmanager.h"

#include "exporter/exporterdialog.h"

#include <QFileDialog>
#include <QGuiApplication>

Exporter::Exporter(MainWindow* mainWindow)
    : mainWin(mainWindow)
{
    mainWin->mainTabWidget->updateTextRangesOfAllEditors();

    showDownloadDialog();
}

inline void Exporter::showDownloadDialog()
{

    QList<FileExportPropertiesResource>& resources =
            mainWin->resourcesManager->getResourcesController<FileExportPropertiesResource>(3)->getResources();

    ExporterDialog dialog(&resources, mainWin);
    dialog.exec();
}
