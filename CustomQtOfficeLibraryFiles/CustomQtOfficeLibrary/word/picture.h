#ifndef PICTURE_H
#define PICTURE_H

class QAxObject;
class QVariant;

namespace OfficeLib { namespace Word {

class Picture
{
public:
    Picture(QAxObject* imgObj);

    //set properties
    void setWidth(const double& width);
    void setHeight(const double& height);
    QVariant getWidth();
    QVariant getHeight();

private:
    QAxObject* imgObj;
    QAxObject* selection;
};

}}

#endif // PICTURE_H
