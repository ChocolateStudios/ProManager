#include "word.h"

#include <QString>
#include <QAxObject>

namespace OfficeLib { namespace Word {

Word::Word() : OfficeLib::Application("Word.Application")
{

}

Document Word::createDocument()
{
    return Document(appObj, appObj->querySubObject("Documents")->querySubObject("Add()"));
    //appObj->querySubObject("Documents")->dynamicCall("Add()");
    //return Document(appObj, appObj->querySubObject("ActiveDocument"));
}

Document Word::openDocument(const QString &fileName)
{
    appObj->querySubObject("Documents")->dynamicCall("Open(const QString&)", fileName);
    return Document(appObj, appObj->querySubObject("ActiveDocument"));
}

}}
