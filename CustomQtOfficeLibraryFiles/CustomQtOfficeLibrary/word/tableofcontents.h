#ifndef TABLEOFCONTENTS_H
#define TABLEOFCONTENTS_H

class QAxObject;

namespace OfficeLib { namespace Word {

class TableOfContents
{
public:
    TableOfContents(QAxObject* tableOO);

    void update();

private:
    QAxObject* tableOfContents;
};

}}

#endif // TABLEOFCONTENTS_H
