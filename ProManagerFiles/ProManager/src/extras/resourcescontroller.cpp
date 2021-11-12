#include "resourcescontroller.h"

#include <QPixmap>
#include <QDataStream>

ResourcesController::ResourcesController()
{
}

void ResourcesController::saveData(QDataStream &out)
{
    //saving images
    out << (int)importedImages.size();
    for (int i = 0; i < importedImages.size(); i++) {
        out << *importedImages[i].pixmap << importedImages[i].path;
    }

    //saving files
    out << (int)importedFiles.size();
    for (int i = 0; i < importedFiles.size(); i++) {
        out << importedFiles[i].path;
    }
}

void ResourcesController::readData(QDataStream &in)
{
    //reading images
    int number;
    in >> number;

    for (int i = 0; i < number; i++) {
        QPixmap* pixmap = new QPixmap;
        QString path;
        in >> *pixmap >> path;
        CustomImage cImage(pixmap, path);
        importedImages.append(cImage);
    }

    //reading files
    in >> number;

    for (int i = 0; i < number; i++) {
        QString path;
        in >> path;
        CustomFile cFile(path);
        importedFiles.append(cFile);
    }
}
