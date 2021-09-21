#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>

namespace OfficeLib { namespace Word {
class Document;
}}
class Style;

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    CustomTextEdit();
    void writeToWord(OfficeLib::Word::Document& wordDocument, QList<Style> &styles);
    void initTest();

public slots:
    void setTextSelectedColor();
    void resetSelectedTextFormat();
    void test();

public:
    QList<Style>* styles;
};

#endif // CUSTOMTEXTEDIT_H
