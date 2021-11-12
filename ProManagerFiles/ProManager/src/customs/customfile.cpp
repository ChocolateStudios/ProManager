#include "customfile.h"

#include <QDir>

CustomFile::CustomFile(QString path_)
    : path(QDir::toNativeSeparators(path_)), name(QFileInfo(path_).fileName())
{

}
