#ifndef FILESDOCK_H
#define FILESDOCK_H

#include "basedock.h"

class QGridLayout;
class CustomFile;

class FilesDock : public BaseDock
{
    Q_OBJECT
    friend class DockController;

private:
    FilesDock(QMainWindow* mainWin, QList<CustomFile>* importedFls);
    bool isUniqueFile(const QString& fileName);
    void addFileImageButton(const CustomFile& currentFile, const QPixmap& fileIcon);

private slots:
    void importFile();

private:

    QWidget* scrollWidget;
    QGridLayout* filesImagesLayout;
    unsigned int row, column;
    QList<CustomFile>* importedFiles;
};

#endif // FILESDOCK_H
