#include "document.h"

#include <QAxObject>

namespace OfficeLib { namespace Word {

Document::Document(QAxObject* appObj, QAxObject *docObj) : wordObj(appObj), docObj(docObj),
    selection(wordObj->querySubObject("Selection")), isOpen(true)
{
}

Document::~Document()
{
    if (isOpen)
        close();
}

void Document::writeText(const QString &text)
{
    if (!isOpen || text.isEmpty()) return;
    selection->dynamicCall("TypeText(const QString&)", text);
}

void Document::setFontSize(const unsigned short &size)
{
    if (!isOpen) return;
    selection->querySubObject("Font")->setProperty("Size", size);
}

void Document::setFontColor(Word::WdColor &color)
{
    if (!isOpen) return;
    selection->querySubObject("Font")->dynamicCall("SetColor(WdColor)", color);
    selection->querySubObject("Font")->setProperty("Color", color);
}

QVariant Document::getFontColor()
{
    if (!isOpen) return QVariant();
    return selection->querySubObject("Font")->dynamicCall("Color()");
}

void Document::setFontName(const QString &font)
{
    if (!isOpen) return;
    selection->querySubObject("Font")->setProperty("Name", font);
}

void Document::setFontBold(const bool &value)
{
    if (!isOpen) return;
    selection->querySubObject("Font")->setProperty("Bold", value);
}

void Document::setParagraphAlignment(WdParagraphAlignment &alignment)
{
    if (!isOpen) return;
    selection->querySubObject("ParagraphFormat")->dynamicCall("SetAlignment(WdParagraphAlignment)", alignment);
    selection->querySubObject("ParagraphFormat")->setProperty("Alignment", alignment);
}

QVariant Document::getParagraphAlignment()
{
    if (!isOpen) return QVariant();
    return selection->querySubObject("ParagraphFormat")->dynamicCall("Alignment()");
}

void Document::setHeader(const unsigned int &headerLevel)
{
    if (!isOpen) return;
    if (headerLevel <= 0)
        selection->setProperty("Style", "Normal");
    else
        selection->setProperty("Style", QString("Heading %1").arg(headerLevel));
}

Picture Document::insertPicture(const QString &filename)
{
    if (!isOpen || filename.isEmpty()) return Picture(nullptr);
    Picture picture = Picture(docObj->querySubObject("InlineShapes()")->querySubObject("AddPicture(QString&)", filename));
    selection->dynamicCall("EndKey()");
    return picture;
}

TableOfContents Document::insertTableOfContents()
{
    if (!isOpen) return TableOfContents(nullptr);
    QAxObject* range = selection->querySubObject("Range");
    QAxObject* tablesOfContents = docObj->querySubObject("TablesOfContents");

    return TableOfContents(tablesOfContents->querySubObject("Add(Range*)", range->asVariant()));
    //tablesOfContents->querySubObject("Add(Word::Range*)", range->asVariant());
}

QVariant Document::getRange()
{
    if (!isOpen) return QVariant();
    return selection->dynamicCall("Range()");
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
