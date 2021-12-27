#include "document.h"

#include "margin.h"
#include "picture.h"
#include "tableofcontents.h"
#include "table.h"
#include "inserter.h"
#include "selectioninserter.h"
#include "rangeinserter.h"

#include <QVariant>
#include <QAxObject>

namespace OfficeLib { namespace Word {

Document::Document(QAxObject* appObj, QAxObject *docObj) : wordObj(appObj), docObj(docObj),
    selection(wordObj->querySubObject("Selection"))
{
    isOpen = false;
    if (wordObj && docObj && selection)
        isOpen = true;
}

Document::~Document()
{
    if (isOpen)
        close();
}

void Document::writeText(Inserter& inserter, const QString &text)
{
    if (!isOpen || text.isEmpty()) return;
    inserter.writeText(text);
//    selection->dynamicCall("TypeText(const QString&)", text);
}

Font Document::getInserterFont(Inserter& inserter)
{
    if (!isOpen) return Font(nullptr);
    return inserter.getFont();
//    return Font(selection->querySubObject("Font"));
}

void Document::setParagraphAlignment(Inserter& inserter, WdParagraphAlignment &alignment)
{
    if (!isOpen) return;
    inserter.setParagraphAlignment(alignment);
//    selection->querySubObject("ParagraphFormat")->dynamicCall("SetAlignment(WdParagraphAlignment)", alignment);
//    selection->querySubObject("ParagraphFormat")->setProperty("Alignment", alignment);
}

QVariant Document::getParagraphAlignment(Inserter& inserter)
{
    if (!isOpen) return QVariant();
    return inserter.getParagraphAlignment();
//    return selection->querySubObject("ParagraphFormat")->dynamicCall("Alignment()");
}

void Document::setHeader(Inserter& inserter, const unsigned int &headerLevel)
{
    if (!isOpen) return;
    inserter.setHeader(headerLevel);
//    if (headerLevel <= 0)
//        selection->setProperty("Style", "Normal");
//    else
//        selection->setProperty("Style", QString("Heading %1").arg(headerLevel));
}

Picture Document::insertPicture(Inserter& inserter, const QString &filename)
{
    if (!isOpen || filename.isEmpty()) return Picture(nullptr);
    return inserter.insertPicture(filename);
//    Picture picture = Picture(docObj->querySubObject("InlineShapes()")->querySubObject("AddPicture(QString&)", filename));
//    selection->dynamicCall("EndKey()");
//    return picture;
}

TableOfContents Document::insertTableOfContents(Inserter& inserter)
{
    if (!isOpen) return TableOfContents(nullptr);
    return inserter.insertTableOfContents();
//    QAxObject* range = selection->querySubObject("Range");
//    QAxObject* tablesOfContents = docObj->querySubObject("TablesOfContents");

//    return TableOfContents(tablesOfContents->querySubObject("Add(Range*)", range->asVariant()));
}

Table Document::insertTable(Inserter& inserter, int rows, int columns)
{
    if (!isOpen) return Table(nullptr, -1, -1);
    return inserter.insertTable(rows, columns);
//    QAxObject* range = selection->querySubObject("Range");
//    QAxObject* tables = selection->querySubObject("Tables");

//    Table table = Table(tables->querySubObject("Add(Range*, int, int)", range->asVariant(), rows, columns));
//    table.setTableStyle("Table Grid");

//    return table;
}

//void Document::copyPages(const unsigned short &startPage, const unsigned short &endPage)
//{
//    if (!isOpen) return;
////    selection->dynamicCall("HomeKey()");
////    QVariant units = 7, movementType = 1;
////    selection->dynamicCall("EndKey(QVariant&, QVariant&)", units, movementType);
//    selection->dynamicCall("WholeStory()");
//    selection->dynamicCall("Copy()");
//}

void Document::paste()
{
    if (!isOpen) return;
    selection->dynamicCall("Paste()");
}

Margin Document::getPageMargin()
{
    if (!isOpen) return Margin();
    double left = -1, right = -1, top = -1, bottom = -1;
    QAxObject* pageSetup = docObj->querySubObject("PageSetup");
    left = pageSetup->dynamicCall("LeftMargin()").toDouble();
    right = pageSetup->dynamicCall("RightMargin()").toDouble();
    top = pageSetup->dynamicCall("TopMargin()").toDouble();
    bottom = pageSetup->dynamicCall("BottomMargin()").toDouble();
    return Margin(left, right, top, bottom);
}

void Document::setPageMargin(Margin newMargin)
{
    if (!isOpen) return;
    QAxObject* pageSetup = docObj->querySubObject("PageSetup");
    if (newMargin.left != -1)
        pageSetup->dynamicCall("SetLeftMargin(double)", newMargin.left);
    if (newMargin.right != -1)
        pageSetup->dynamicCall("SetRightMargin(double)", newMargin.right);
    if (newMargin.top != -1)
        pageSetup->dynamicCall("SetTopMargin(double)", newMargin.top);
    if (newMargin.bottom != -1)
        pageSetup->dynamicCall("SetBottomMargin(double)", newMargin.bottom);
}

Inserter* Document::getSelectionInserter()
{
    return new SelectionInserter(selection, docObj);
}

Inserter *Document::getRangeInserter(QAxObject *range)
{
    return new RangeInserter(range, docObj);
}

Inserter *Document::getSelectionRangeInserter()
{
    return new RangeInserter(selection->querySubObject("Range"), docObj);
}

void Document::moveEndSelection(WdUnits unit, WdMovementType movement)
{
    if (!isOpen) return;
    selection->dynamicCall("EndKey(QVariant&, QVariant&)", unit, movement);
}

void Document::findAndReplace(QString findText, QString replaceWithText)
{
    if (!isOpen) return;

    bool matchCase = false;
    bool matchWholeWord = true;
    bool matchWildCards = false;
    bool matchSoundsLike = false;
    bool matchAllWordForms = false;
    bool forward = true;
    int wrap = 1;
    bool format = false;
    int replace = 2;

    QVariantList vList = { findText, matchCase, matchWholeWord, matchWildCards, matchSoundsLike, matchAllWordForms,
                           forward, wrap, format, replaceWithText, replace };

    auto findAndReplaceFunc = [&](QAxObject* range) {
        range->querySubObject("Find")->dynamicCall("ClearFormatting");
        range->querySubObject("Find")->
            dynamicCall("Execute(QVariant&, QVariant&, QVariant&, QVariant&, QVariant&, QVariant&, QVariant&, QVariant&, QVariant&, QVariant&, QVariant&)",
                        vList);
    };

    findAndReplaceFunc(selection->querySubObject("Range"));

    QAxObject* storyRanges = docObj->querySubObject("StoryRanges");

    for (int storyType = 1; storyType < 18; storyType++) {
        QAxObject* cRange = storyRanges->querySubObject("Item(int)", storyType);

        while(cRange && !cRange->isNull()) {
            findAndReplaceFunc(cRange);
            cRange = cRange->querySubObject("NextStoryRange()");
        }
    }

}

Inserter* Document::find(QString findText)
{
    if (!isOpen) return nullptr;

    bool matchCase = false;
    bool matchWholeWord = true;
    bool matchWildCards = false;
    bool matchSoundsLike = false;
    bool matchAllWordForms = false;
    bool forward = true;
    int wrap = 1;
    bool format = false;
    const QString replaceWithText = "";
    int replace = 0;

    QVariantList vList = { findText, matchCase, matchWholeWord, matchWildCards, matchSoundsLike, matchAllWordForms,
                           forward, wrap, format, replaceWithText, replace };

    auto findFunc = [&](QAxObject* range) {
        range->querySubObject("Find")->dynamicCall("ClearFormatting()");
        return range->querySubObject("Find")->
            dynamicCall("Execute(QVariant&, QVariant&, QVariant&, QVariant&, QVariant&, QVariant&, QVariant&, QVariant&, QVariant&, QVariant&, QVariant&)",
                        vList).toBool();
    };

    QAxObject* curRange = selection->querySubObject("Range");
    if (findFunc(curRange))
        return new RangeInserter(curRange, docObj);

    QAxObject* storyRanges = docObj->querySubObject("StoryRanges");

    for (int storyType = 1; storyType < 18; storyType++) {
        QAxObject* cRange = storyRanges->querySubObject("Item(int)", storyType);

        while(cRange && !cRange->isNull()) {
            if (findFunc(cRange))
                return new RangeInserter(cRange, docObj);
            cRange = cRange->querySubObject("NextStoryRange()");
        }
    }
    return nullptr;
}

void Document::close()
{
    if (!isOpen) return;
    docObj->dynamicCall("Close(bool)", false);
    isOpen = false;
}

void Document::save()
{
    if (!isOpen) return;
     docObj->dynamicCall("Save()");
}

void Document::saveAs(const QString &fileName)
{
    if (!isOpen) return;
    docObj->dynamicCall("SaveAs(const QString&)", fileName);
}

}}
