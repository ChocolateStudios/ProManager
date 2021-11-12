#ifndef TEXTANALIZER_H
#define TEXTANALIZER_H

#include <QList>
#include <QTextCursor>
#include <QMap>

class Style;
class QColor;
class CustomImage;
class QStandardItemModel;
class QStandardItem;
class QAction;

namespace OfficeLib { namespace Word {
    class Document;
}}

struct TextRange {
    int start, end;
};

class TextAnalizer
{
public:
    TextAnalizer();
    void writeToWord(OfficeLib::Word::Document& wordDocument, QTextCursor textCursor, const QList<Style>& styles, const QList<CustomImage>& importedImages, std::function<void(const Style*)> setTextPropertiesFunc);
    bool containsSpecialText(QTextCursor& textCursor);
    void removeUnderlinedSpecialText();
    void restoreOverridedCursor();
    void setTextSelectedColor(QTextCursor textCursor, const QList<Style> &styles, QAction* act);
    void setNavegationTexts(QStandardItemModel* model, const QStandardItemModel* otherModel, QTextCursor textCursor);
    void appendNextNavegationLevel(const QStandardItemModel *otherModel, QStandardItem* lastLevel, int levelIndex, int lastLevelItemIndex, QTextCursor& textCursor, int lastEndIndex, int maxStringLength);
    void updateBackgroundStyleColor(const Style &style, const QList<Style> &styles, QTextCursor textCursor, const QString& previousName);

    void saveData(QDataStream &out);
    void readData(QDataStream &in, const QList<Style> &styles, QTextCursor textCursor);

private:
    const Style* containsStyleByColor(const QColor& col, const QList<Style>& styles) const;
    const Style* containsStyleByName(const QString& name, const QList<Style>& styles) const;
    bool checkAndResolveRangeCollision(const QString& styleName, TextRange& compareRange);

    bool overridedCursor = false;
    QMap<QString, QList<TextRange>> styleForRangeInText;

public:
    QTextCursor specialTextSelectedCursor;
};

#endif // TEXTANALIZER_H
