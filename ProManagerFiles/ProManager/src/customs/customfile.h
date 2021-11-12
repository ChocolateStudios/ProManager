#ifndef CUSTOMFILE_H
#define CUSTOMFILE_H

#include <QString>


class CustomFile
{
public:
    CustomFile();
    CustomFile(QString path_);

    QString path;
    QString name;
};

#endif // CUSTOMFILE_H
