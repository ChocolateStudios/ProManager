#ifndef DOCUMENT_H
#define DOCUMENT_H

class QAxObject;
class QString;
class QColor;
class QVariant;

namespace OfficeLib { namespace Word {

enum WdColor {
    wdColorAutomatic        = -16777216,
    wdColorBlack            = 0,
    wdColorBlue             = 16711680,
    wdColorTurquoise        = 16776960,
    wdColorBrightGreen      = 65280,
    wdColorPink             = 16711935,
    wdColorRed              = 255,
    wdColorYellow           = 65535,
    wdColorWhite            = 16777215,
    wdColorDarkBlue         = 8388608,
    wdColorTeal             = 8421376,
    wdColorGreen            = 32768,
    wdColorViolet           = 8388736,
    wdColorDarkRed          = 128,
    wdColorDarkYellow       = 32896,
    wdColorBrown            = 13209,
    wdColorOliveGreen       = 13107,
    wdColorDarkGreen        = 13056,
    wdColorDarkTeal         = 6697728,
    wdColorIndigo           = 10040115,
    wdColorOrange           = 26367,
    wdColorBlueGray         = 10053222,
    wdColorLightOrange      = 39423,
    wdColorLime             = 52377,
    wdColorSeaGreen         = 6723891,
    wdColorAqua             = 13421619,
    wdColorLightBlue        = 16737843,
    wdColorGold             = 52479,
    wdColorSkyBlue          = 16763904,
    wdColorPlum             = 6697881,
    wdColorRose             = 13408767,
    wdColorTan              = 10079487,
    wdColorLightYellow      = 10092543,
    wdColorLightGreen       = 13434828,
    wdColorLightTurquoise   = 16777164,
    wdColorPaleBlue         = 16764057,
    wdColorLavender         = 16751052,
    wdColorGray05           = 15987699,
    wdColorGray10           = 15132390,
    wdColorGray125          = 14737632,
    wdColorGray15           = 14277081,
    wdColorGray20           = 13421772,
    wdColorGray25           = 12632256,
    wdColorGray30           = 11776947,
    wdColorGray35           = 10921638,
    wdColorGray375          = 10526880,
    wdColorGray40           = 10066329,
    wdColorGray45           = 9211020,
    wdColorGray50           = 8421504,
    wdColorGray55           = 7566195,
    wdColorGray60           = 6710886,
    wdColorGray625          = 6316128,
    wdColorGray65           = 5855577,
    wdColorGray70           = 5000268,
    wdColorGray75           = 4210752,
    wdColorGray80           = 3355443,
    wdColorGray85           = 2500134,
    wdColorGray875          = 2105376,
    wdColorGray90           = 1644825,
    wdColorGray95           = 789516
};
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

class Document
{
public:
    Document(QAxObject* appObj, QAxObject* docObj);
    ~Document();
    void writeText(const QString& text);

    // Font Properties
    void setFontSize(const unsigned short& size);
    void setFontColor(WdColor& color);
    QVariant getFontColor();
    void setFontName(const QString& font);
    void setFontBold(const bool& value);

    //Paragraph Properties
    void setParagraphAlignment(WdParagraphAlignment& alignment);
    QVariant getParagraphAlignment();

    void close();
    void save();
    void saveAs(const QString& fileName);

private:
    QAxObject* wordObj;
    QAxObject* docObj;
    QAxObject* selection;
    bool isOpen;
};

}}

#endif // DOCUMENT_H
