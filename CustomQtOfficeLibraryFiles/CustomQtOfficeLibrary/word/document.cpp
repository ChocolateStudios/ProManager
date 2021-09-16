#include "document.h"

#include <QAxObject>

namespace OfficeLib { namespace Word {

Document::Document(QAxObject* appObj, QAxObject *docObj) : wordObj(appObj), docObj(docObj), isOpen(true)
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
    QAxObject* selection = wordObj->querySubObject("Selection");
    selection->dynamicCall("TypeText(const QString&)", text);
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
