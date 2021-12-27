#include "textdocumenttaganalizer.h"

#include "resources/manager/resourcesmanager.h"

#include "converters/wordconverter.h"
#include "converters/powerpointconverter.h"
#include "converters/excelconverter.h"

#include "word/document.h"
#include "word/picture.h"
#include "word/tableofcontents.h"
#include "word/rangeinserter.h"

TextDocumentTagAnalizer::TextDocumentTagAnalizer(BaseConverter* converter, ResourcesManager *resManager, OfficeLib::Word::Inserter* inserter, QList<OfficeLib::Word::TableOfContents>& tablesToUpdate)
    : TagAnalizer(converter, resManager), inserter(inserter), tablesToUpdate(tablesToUpdate)
{

}

void TextDocumentTagAnalizer::doSomethingInWord(WordConverter* converter)
{
    OfficeLib::Word::Document* wordDocument = converter->document;

    //TYPE IN LOWERCASE
    if (type == "img") {
        imgTag(wordDocument);
    }

    else if (type == "pagebreak") {
        pageBreakTag(wordDocument);
    }

    else if (type == "file") {
        fileTag(wordDocument, converter);
    }

    else if (type == "tableofcontent" || type == "tableofcontents") {
        tableOfContentTag(wordDocument);
    }
}

void TextDocumentTagAnalizer::doSomethingInExcel(ExcelConverter* converter)
{

}

inline void TextDocumentTagAnalizer::imgTag(OfficeLib::Word::Document *wordDocument)
{

    if (!properties["src"].isEmpty()) {
        QString imageName = properties["src"];
        QString imagePath;

        if (ImageResource* imageRes = resManager->getResourcesController<ImageResource>(1)->getResourceByName(imageName)) {
            imagePath = imageRes->getPath();
        }
        else
            return;

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

            wordDocument->setParagraphAlignment(*inserter, alignment);
        }

        OfficeLib::Word::Picture picture = wordDocument->insertPicture(*inserter, imagePath);
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

inline void TextDocumentTagAnalizer::pageBreakTag(OfficeLib::Word::Document *wordDocument)
{
    bool ok = false;
    int num = properties["n"].toInt(&ok);
    num = ok ? num : 1;

    for (int i = 0; i < num; i++) {
        wordDocument->writeText(*inserter, "\f");
        if (OfficeLib::Word::RangeInserter* rangeInserter = dynamic_cast<OfficeLib::Word::RangeInserter*>(inserter))
            rangeInserter->afterWriteText();
    }
}

void TextDocumentTagAnalizer::fileTag(OfficeLib::Word::Document *wordDocument, WordConverter *converter)
{
    bool canContinue = true;

    for (int i = 0; canContinue; i++) {
        const QString& complement = i == 0 ? "" : QString("%1").arg(i);
        canContinue = false;

        if (!properties[QString("find%1").arg(complement)].isEmpty()) {
            QString findKeyWord = properties[QString("find%1").arg(complement)];

            if (!properties[QString("replace%1").arg(complement)].isEmpty()) {
                canContinue = true;
                QString replaceText = properties[QString("replace%1").arg(complement)];

                if (replaceText.endsWith(".xd") || replaceText.endsWith(".uwu")) {
                    OfficeLib::Word::Inserter* newInserter = wordDocument->find(findKeyWord);
                    converter->fillDocumentWithEditorName(replaceText, newInserter, tablesToUpdate);
                }
                else if (replaceText.toLower() == "tableofcontent" || replaceText.toLower() == "tableofcontents") {
                    OfficeLib::Word::Inserter* newInserter = wordDocument->find(findKeyWord);
                    OfficeLib::Word::TableOfContents tableOfContents = wordDocument->insertTableOfContents(*newInserter);
                    tablesToUpdate.append(tableOfContents);
                }
                else {
                    wordDocument->findAndReplace(findKeyWord, replaceText);
                }
            }
        }
    }

    wordDocument->moveEndSelection(OfficeLib::Word::WdUnits::wdStory, OfficeLib::Word::WdMovementType::wdMove);
}

inline void TextDocumentTagAnalizer::tableOfContentTag(OfficeLib::Word::Document *wordDocument)
{
    OfficeLib::Word::TableOfContents tableOfContents = wordDocument->insertTableOfContents(*inserter);
    tablesToUpdate.append(tableOfContents);
}
