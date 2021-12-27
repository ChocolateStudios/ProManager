#ifndef TEXTRANGESCONTROLLER_H
#define TEXTRANGESCONTROLLER_H

#include <QMap>
#include <QString>

class TextRange;
class QTextCursor;
class StyleResource;
class StyleResourcesController;

class TextRangesController
{
    friend QDataStream &operator>>(QDataStream& in, TextRangesController controller);
    friend QDataStream &operator<<(QDataStream& out, TextRangesController controller);

public:
    TextRangesController(StyleResourcesController* styleResourcesController);
    void appendTextRange(const QString& styleName, TextRange& textRange);
    void paintTextRanges(QTextCursor &textCursor, std::function<const StyleResource*(const QString&)> getStyleByName);
    void debugTextRanges();

    const QList<TextRange>& getTextRangesByStyleName(const QString& styleName);

    void addQuantityToStyleRanges(QTextCursor& textCursor);
    void removeQuantityToStyleRanges(QTextCursor& textCursor);

    inline void addCountToAdd(int countToAddToRanges, QTextCursor& textCursor) {
        if (countToAdd > 0 && countToAddToRanges < 0)
            addQuantityToStyleRanges(textCursor);
        countToAdd += countToAddToRanges;
    }
    inline void setStartPosition(int startPos) {
        if (startPosition >= 0)
            return;
        startPosition = startPos;
    }

private:
    StyleResourcesController* styleResourcesController;
    QMap<QString, QList<TextRange>> styleRanges;
    int countToAdd = 0;
    int startPosition = -1;
};

#endif // TEXTRANGESCONTROLLER_H
