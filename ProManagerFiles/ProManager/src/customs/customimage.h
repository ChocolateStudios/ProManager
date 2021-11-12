#ifndef CUSTOMIMAGE_H
#define CUSTOMIMAGE_H

#include <QString>

class QPixmap;

class CustomImage
{
public:
    CustomImage();
    CustomImage(QPixmap* pixmap, QString path);

    QPixmap* pixmap;
    QString path;
    QString name;
};

#endif // CUSTOMIMAGE_H
