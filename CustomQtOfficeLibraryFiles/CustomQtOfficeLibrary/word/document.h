#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "wordenums.h"

class QAxObject;
class QString;
class QColor;
class QVariant;

namespace OfficeLib { namespace Word {

class Font;
class Picture;
class TableOfContents;
class Table;
class Margin;
class Inserter;

class Document
{
public:
    Document(QAxObject* appObj, QAxObject* docObj);
    Document() {}
    ~Document();

    void writeText(Inserter& inserter, const QString& text);

    Font getInserterFont(Inserter& inserter);

    void setParagraphAlignment(Inserter& inserter, WdParagraphAlignment& alignment);
    QVariant getParagraphAlignment(Inserter& inserter);

    void setHeader(Inserter& inserter, const unsigned int& headerLevel);

    Picture insertPicture(Inserter& inserter, const QString& filename);
    TableOfContents insertTableOfContents(Inserter& inserter);
    Table insertTable(Inserter& inserter, int rows = 1, int columns = 1);

//    void copyPages(const unsigned short& startPage, const unsigned short& endPage);
    void paste();

    Margin getPageMargin();
    void setPageMargin(Margin newMargin);

    Inserter* getSelectionInserter();
    Inserter* getRangeInserter(QAxObject* range);
    Inserter* getSelectionRangeInserter();

    void moveEndSelection(WdUnits unit, WdMovementType movement);

    void findAndReplace(QString findText, QString replaceWithText);
    Inserter* find(QString findText);

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
