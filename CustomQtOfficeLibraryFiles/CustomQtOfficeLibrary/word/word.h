#ifndef WORD_H
#define WORD_H

#include "application.h"
#include "document.h"

namespace OfficeLib { namespace Word {

class Word : public Application
{
public:
    Word();
    Document* createDocument();
    Document* openDocument(const QString& fileName);
};

}}

#endif // WORD_H
