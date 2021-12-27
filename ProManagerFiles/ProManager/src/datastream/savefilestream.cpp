#include "savefilestream.h"

#include "mainwindow.h"
#include "maintabwidget.h"

#include "resources/manager/resourcesmanager.h"

#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QSaveFile>

SaveFileStream::SaveFileStream(QSaveFile& file, const QString &_filename, QString &errorMessage, MainWindow *mainWin)
    : filename(_filename), mainWin(mainWin), file(file)
{
    if (file.open(QFile::WriteOnly)) {
        if (QFileInfo(filename).suffix() == "choco") {
            saveChocoFile();
        }
        if (!file.commit())
            errorMessage = QObject::tr("Cannot write file %1:\n%2.").arg(QDir::toNativeSeparators(filename), file.errorString());
    }
    else
        errorMessage = QObject::tr("Cannot open file %1 for writing:\n%2.").arg(QDir::toNativeSeparators(filename), file.errorString());

}

void SaveFileStream::saveChocoFile()
{
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint32(mainWin->MagicNumber);

    mainWin->resourcesManager->saveResources(out);
    mainWin->mainTabWidget->saveEditorsContent(out);

    QGuiApplication::restoreOverrideCursor();
}
