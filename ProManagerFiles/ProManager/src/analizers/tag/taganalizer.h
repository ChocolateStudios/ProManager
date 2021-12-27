#ifndef TAGANALIZER_H
#define TAGANALIZER_H

#include <QString>
#include <QMap>

class BaseConverter;
class WordConverter;
class ExcelConverter;
class ResourcesManager;
class QTextCursor;

class TagAnalizer
{
public:
    TagAnalizer(const QString& text);
    TagAnalizer(BaseConverter* converter, ResourcesManager* resManager);

    void doSomething(const QString& tagText);
    static bool containsTag(QTextCursor& textCursor);

    int getDockIdByType();
    const QString& getPropertyValue(const QString& propertyKey);

protected:
    void readText(const QString& text);
    void clearText();
    void jumpToNextWord(unsigned int& index, const QString& text);
    void readType(unsigned int& index, const QString& text);;
    void readProperty(unsigned int& index, const QString& text);

    virtual void doSomethingInWord(WordConverter* converter) {}
    virtual void doSomethingInExcel(ExcelConverter* converter) {}

    QString type;
    QMap<QString, QString> properties;
    BaseConverter* converter;
    ResourcesManager* resManager;
};

#endif // TAGANALIZER_H
