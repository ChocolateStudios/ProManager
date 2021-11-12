#ifndef SPECIALTEXTTAG_H
#define SPECIALTEXTTAG_H

#include <QString>
#include <QMap>

class CustomImage;

namespace OfficeLib { namespace Word {
    class Document;
    class TableOfContents;
}}

class SpecialTextTag
{
public:
    SpecialTextTag(const QString& specialText);
    void doSomething(OfficeLib::Word::Document& wordDocument, const QList<CustomImage>& importedImages, QList<OfficeLib::Word::TableOfContents>& tablesToUpdate);

private:
    void jumpNextWord(const QString& text, int& i);
    void readType(const QString& text, int& i);
    void readProperty(const QString& text, int& i);

    QString type;
    QMap<QString, QString> properties;
};

#endif // SPECIALTEXTTAG_H
