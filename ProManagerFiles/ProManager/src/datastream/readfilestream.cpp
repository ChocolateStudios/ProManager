#include "readfilestream.h"

#include "mainwindow.h"
#include "maintabwidget.h"

#include "resources/manager/resourcesmanager.h"

#include <QFileInfo>
#include <QGuiApplication>
#include <QMessageBox>

ReadFileStream::ReadFileStream(QFile& file, const QString &filename, MainWindow* mainWin)
    : file(file), filename(filename), mainWin(mainWin)
{
    const QString& suffix = QFileInfo(filename).suffix();
    if (suffix == "choco") {
        readChocoFile();
    }
}

void ReadFileStream::readChocoFile()
{
    QDataStream in(&file);

    in.setVersion(QDataStream::Qt_6_2);
    quint32 magic;
    in >> magic;

    if (magic != mainWin->MagicNumber) {
        QMessageBox::warning(mainWin, QCoreApplication::applicationName(), QObject::tr("The file is not a ProManager file."));
        return;
    }

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);

    mainWin->resourcesManager->readResources(in, mainWin->mainTabWidget->stylesContextMenu);
    mainWin->mainTabWidget->readEditorsContent(in);

    QGuiApplication::restoreOverrideCursor();

}
