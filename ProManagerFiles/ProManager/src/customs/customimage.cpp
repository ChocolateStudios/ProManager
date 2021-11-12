#include "customimage.h"

#include <QFileInfo>
#include <QPixmap>
#include <QDir>

CustomImage::CustomImage()
{

}

CustomImage::CustomImage(QPixmap *pixmap, QString path_)
    : pixmap(pixmap), path(QDir::toNativeSeparators(path_)), name(QFileInfo(path_).fileName())
{
}
