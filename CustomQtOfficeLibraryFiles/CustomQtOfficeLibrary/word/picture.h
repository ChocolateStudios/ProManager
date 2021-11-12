#ifndef PICTURE_H
#define PICTURE_H

class QAxObject;
class QVariant;

namespace OfficeLib { namespace Word {

enum WdParagraphAlignment {
    wdAlignParagraphLeft    = 0,
    wdAlignParagraphCenter  = 1,
    wdAlignParagraphRight   = 2,
    wdAlignParagraphJustify = 3,
    wdAlignParagraphDistribute= 4,
    wdAlignParagraphJustifyMed= 5,
    wdAlignParagraphJustifyHi= 7,
    wdAlignParagraphJustifyLow= 8,
    wdAlignParagraphThaiJustify= 9
};

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
