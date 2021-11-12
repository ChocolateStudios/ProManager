#include "picture.h"

#include <QAxObject>

namespace OfficeLib { namespace Word {

Picture::Picture(QAxObject *imgObj)
    : imgObj(imgObj)
{
}

void Picture::setWidth(const double& width)
{
    if (!imgObj) return;
    imgObj->setProperty("Width", width);
}

void Picture::setHeight(const double& height)
{
    if (!imgObj) return;
    imgObj->setProperty("Height", height);
}

QVariant Picture::getWidth()
{
    if (!imgObj) return QVariant();
    return imgObj->dynamicCall("Width()");
}

QVariant Picture::getHeight()
{
    if (!imgObj) return QVariant();
    return imgObj->dynamicCall("Height()");
}

}}
