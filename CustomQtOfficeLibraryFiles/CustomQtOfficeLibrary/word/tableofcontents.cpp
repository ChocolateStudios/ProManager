#include "tableofcontents.h"

#include <QAxObject>

namespace OfficeLib { namespace Word {

TableOfContents::TableOfContents(QAxObject *tableOO)
    : tableOfContents(tableOO)
{

}

void TableOfContents::update()
{
    if (!tableOfContents) return;
    tableOfContents->dynamicCall("Update()");
}

}}
