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
    void writeToWord(OfficeLib::Word::Document& wordDocument, const QList<Style> &styles);
    void initTest();
    void saveData(QDataStream& out);
    void readData(QDataStream& in);

public slots:
    void setTextSelectedColor();
    void resetSelectedTextFormat();
    void test();
};

#endif // CUSTOMTEXTEDIT_H
