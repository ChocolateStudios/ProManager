#ifndef DOCUMENT_H
#define DOCUMENT_H

class QAxObject;
class QString;

namespace OfficeLib { namespace Word {

class Document
{
public:
    Document(QAxObject* appObj, QAxObject* docObj);
    ~Document();
    void writeText(const QString& text);
    void close();
    void save();
    void saveAs(const QString& fileName);

private:
    QAxObject* wordObj;
    QAxObject* docObj;
    bool isOpen;
};

}}

#endif // DOCUMENT_H
