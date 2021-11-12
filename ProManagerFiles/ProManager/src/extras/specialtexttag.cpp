#include "specialtexttag.h"

#include "customs/customimage.h"

#include "word/document.h"
#include "word/picture.h"

#include <QDebug>
#include <QVariant>

SpecialTextTag::SpecialTextTag(const QString& specialText) : type("")
{
    int i = 1;
    jumpNextWord(specialText, i);
    readType(specialText, i);
    jumpNextWord(specialText, i);

    while (i < specialText.size() - 1) {
        readProperty(specialText, i);
        jumpNextWord(specialText, i);
    }
}

void SpecialTextTag::doSomething(OfficeLib::Word::Document& wordDocument, const QList<CustomImage>& importedImages, QList<OfficeLib::Word::TableOfContents>& tablesToUpdate)
{
    if (type == "img") {        //TYPE IN LOWERCASE
        if (!properties["src"].isEmpty()) {
            QString imageName = properties["src"];
            bool canContinue = false;
            for (const CustomImage& c : importedImages) {
                if (c.name == imageName){
                    imageName = c.path;
                    canContinue = true;
                    break;
                }
            }
            if (!canContinue) return;

            if (!properties["alignment"].isEmpty()) {
                QString alignmentStr = properties["alignment"];
                OfficeLib::Word::WdParagraphAlignment alignment = OfficeLib::Word::WdParagraphAlignment::wdAlignParagraphJustify;
                if (alignmentStr == "left") {
                    alignment = OfficeLib::Word::WdParagraphAlignment::wdAlignParagraphLeft;
                }
                else if (alignmentStr == "center") {
                    alignment = OfficeLib::Word::WdParagraphAlignment::wdAlignParagraphCenter;
                }
                else if (alignmentStr == "right") {
                    alignment = OfficeLib::Word::WdParagraphAlignment::wdAlignParagraphRight;
                }
                wordDocument.setParagraphAlignment(alignment);
            }

            OfficeLib::Word::Picture picture = wordDocument.insertPicture(imageName);
            bool ok = false;

            QString widthStr = properties["width"];
            QString heightStr = properties["height"];
            bool lockAspectRatio = false;

            if (widthStr.isEmpty() || heightStr.isEmpty())
                lockAspectRatio = true;

            if (!widthStr.isEmpty()) {
                double width = widthStr.toDouble(&ok);
                if (ok) {
                    width = 10000 / (352.78 / width);
                    if (lockAspectRatio)
                        picture.setHeight((width / picture.getWidth().toDouble()) * picture.getHeight().toDouble());
                    picture.setWidth(width);
                }
            }

            if (!heightStr.isEmpty()) {
                double height = heightStr.toDouble(&ok);
                if (ok) {
                    height = 10000 / (352.78 / height);
                    if (lockAspectRatio)
                        picture.setWidth((height / picture.getHeight().toDouble()) * picture.getWidth().toDouble());
                    picture.setHeight(height);
                }
            }
        }
    }

    else if (type == "pagebreak") {
        bool ok = false;
        int num = properties["n"].toInt(&ok);
        num = ok ? num : 1;

        for (int i = 0; i < num; i++)
            wordDocument.writeText("\f");
    }

    else if (type == "tableofcontent") {
        OfficeLib::Word::TableOfContents tableOfContents = wordDocument.insertTableOfContents();
        tablesToUpdate.append(tableOfContents);
    }
}

inline void SpecialTextTag::jumpNextWord(const QString &text, int &i)
{
    while (i < text.size() - 1 && text[i] == ' ') {
        i++;
    }
}

inline void SpecialTextTag::readType(const QString &text, int &i)
{
    while (i < text.size() - 1 && text[i] != ' ') {
        type += text[i];
        i++;
    }
    type = type.toLower();
}

inline void SpecialTextTag::readProperty(const QString &text, int &i)
{
    QString propKey = "";
    QString propValue = "";

    while (i < text.size() - 1 && text[i] != ' ' && text[i] != '=') {
        propKey += text[i];
        i++;
    }

    if (text[i] == ' ')
        jumpNextWord(text, i);

    if (text[i] == '=') {
        i++;
        if (text[i] == ' ')
            jumpNextWord(text, i);
    }
    else
        return; //Maybe throw error

    if (text[i] == '\"')
        i++;
    else
        return;

    bool propertyFinished = false;
    while (i < text.size() - 1 && text[i] != ' ') {
        if (text[i] == '\"') {
            i++;
            propertyFinished = true;
            break;
        }
        propValue += text[i];
        i++;
    }

    if (propKey.isEmpty() || propValue.isEmpty() || !propertyFinished)
        return;

    propKey = propKey.toLower();

    properties.insert(propKey, propValue);
}
