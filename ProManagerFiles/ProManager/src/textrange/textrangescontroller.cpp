#include "textrangescontroller.h"

#include "textrange/textrange.h"

#include "resources/styleresource.h"
#include "resources/controllers/styleresourcescontroller.h"

#include "sort/heap.h"

#include <QDebug>
#include <QTextCursor>

TextRangesController::TextRangesController(StyleResourcesController *styleResourcesController)
    : styleResourcesController(styleResourcesController)
{

}

void TextRangesController::appendTextRange(const QString &styleName, TextRange &newTextRange)
{
    const QList<QString>& keys = styleRanges.keys();
    int size = keys.size();
    for (int i = 0; i < size; i++) {
        if (keys[i] == styleName)
            continue;

        QList<TextRange>& textRanges = styleRanges[keys[i]];
        int txRngSize = textRanges.size();

        for (int j = 0; j < txRngSize; j++) {
            TextRange& cRange = textRanges[j];

            if (cRange.end() < newTextRange.start)
                continue;
            else if (cRange.start > newTextRange.end())
                break;

            int startDifference = cRange.start - newTextRange.start;
            int endDifference = cRange.end() - newTextRange.end();

            if (startDifference >= 0 && endDifference > 0) {
                // Modificar solo START
                int offset = newTextRange.end() - cRange.start;
                cRange.addToStart(offset);
                cRange.addToLength(-offset);
            }
            else if (startDifference < 0 && endDifference <= 0) {
                // Modificar solo END
                int offset = cRange.end() - newTextRange.start;
                cRange.addToLength(-offset);
            }
            else if (startDifference >= 0 && endDifference <= 0) {
                textRanges.remove(j);
                txRngSize--;
                j--;
                continue;
            }
            else if (startDifference < 0 && endDifference > 0) {
                // Dividirlo en 2
                //Save end
                int lastEnd = cRange.end();

                //Change current Length
                cRange.setEnd(newTextRange.start);

                //Create another TextRange
                TextRange newTextRangeFromDivision(newTextRange.end(), lastEnd);
                textRanges.insert(j + 1, newTextRangeFromDivision);
                txRngSize++;
                j++;
            }
            if (cRange.lenght == 0) {
                textRanges.remove(j);
                txRngSize--;
                j--;
            }
        }
    }

    if (styleResourcesController->getCommentStyleName() == styleName)
        return;

    if (styleRanges.contains(styleName)) {
        QList<TextRange>& textRanges = styleRanges[styleName];
        int txRngSize = textRanges.size();
        bool rangeInserted = false;

        TextRange* txRngPtr = &newTextRange;

        for (int i = 0; i < txRngSize; i++) {
            TextRange& cRange = textRanges[i];

            if (!rangeInserted && cRange.start >= txRngPtr->start) {
                rangeInserted = true;
                textRanges.insert(i, *txRngPtr);
                txRngPtr = &textRanges[i];
                txRngSize++;
                i++;
            }

            if (cRange.end() < txRngPtr->start)
                continue;
            else if (cRange.start > txRngPtr->end())
                break;

            int startDifference = cRange.start - txRngPtr->start;
            int endDifference = cRange.end() - txRngPtr->end();

            if (startDifference >= 0 && endDifference > 0) {
                // Modificar solo END
                txRngPtr->setEnd(cRange.end());
            }
            else if (startDifference < 0 && endDifference <= 0) {
                // Modificar solo START
                int lastEnd = txRngPtr->end();
                txRngPtr->start = cRange.start;
                txRngPtr->setEnd(lastEnd);   //To change length
            }
            else if (startDifference < 0 && endDifference > 0) {
                // Modificar todo
                txRngPtr->start = cRange.start;
                txRngPtr->lenght = cRange.lenght;
            }

            textRanges.remove(i);
            txRngSize--;
            i--;
        }

        if (!rangeInserted)
            textRanges.append(*txRngPtr);
    }
    else {
        QList<TextRange> ranges;
        ranges.append(newTextRange);
        styleRanges.insert(styleName, ranges);
    }
}

void TextRangesController::paintTextRanges(QTextCursor &textCursor, std::function<const StyleResource*(const QString&)> getStyleByName)
{
    QList<QString> keys = styleRanges.keys();
    int keysSize = keys.size();
    for (int j = 0; j < keysSize; j++) {
        if (const StyleResource* style = getStyleByName(keys[j])) {
            QList<TextRange>& ranges = styleRanges[keys[j]];
            int rangesSize = ranges.size();
            for (int w = 0; w < rangesSize; w++) {
                textCursor.setPosition(ranges[w].start, QTextCursor::MoveAnchor);
                textCursor.setPosition(ranges[w].end(), QTextCursor::KeepAnchor);

                QTextCharFormat charFormat = textCursor.charFormat();
                charFormat.setBackground(style->getIconColor());
                textCursor.setCharFormat(charFormat);
            }
        }
    }
}

void TextRangesController::debugTextRanges()
{
    QList<QString> keys = styleRanges.keys();
    for (int i = 0; i < keys.size(); i++) {
        QString debugText = keys[i] + ": ";
        for (int j = 0; j < styleRanges[keys[i]].size(); j++) {
            debugText += QString("[%1, %2] - %3%4").
                    arg(styleRanges[keys[i]][j].start).
                    arg(styleRanges[keys[i]][j].end()).
                    arg(styleRanges[keys[i]][j].lenght).
                    arg(j == styleRanges[keys[i]].size() - 1 ? "" : ", ");
        }
        qDebug() << debugText;
    }
    qDebug() << "-----------------";
}

const QList<TextRange> &TextRangesController::getTextRangesByStyleName(const QString &styleName)
{
    return styleRanges[styleName];
}

void TextRangesController::addQuantityToStyleRanges(QTextCursor& textCursor)
{
    if (countToAdd <= 0) {
        startPosition = -1;
        return;
    }

    QList<TextRange*> candidates;

    QList<QString> keys = styleRanges.keys();
    int keysSize = keys.size();
    for (int i = 0; i < keysSize; i++) {
        QList<TextRange>& textRanges = styleRanges[keys[i]];
        int txRngSize = textRanges.size();

        for (int j = 0; j < txRngSize; j++) {
            TextRange& cRange = textRanges[j];

            if (cRange.end() < startPosition)
                continue;

            if (cRange.start <= startPosition && cRange.end() >= startPosition) {
                candidates.append(&cRange);
                continue;
            }

            cRange.addToStart(countToAdd);
        }
    }

    int candidatesSize = candidates.size();
    if (candidatesSize > 1) {

        heapSort<TextRange*>(candidates, candidates.size(), [&](TextRange* a, TextRange* b) {
            return a->start < b->start;
        });
    }

    if (candidatesSize > 0) {
        candidates[0]->addToLength(countToAdd);

        for (int i = 1; i < candidatesSize; i++) {
            candidates[i]->addToStart(countToAdd);
        }
    }

    countToAdd = 0;
    startPosition = -1;

    debugTextRanges();
}

void TextRangesController::removeQuantityToStyleRanges(QTextCursor& textCursor)
{
    if (countToAdd >= 0) {
        startPosition = -1;
        return;
    }

    int endPosition = startPosition + countToAdd;

    auto removeRangeFunc = [&](const QString& styleName, QList<TextRange>& textRanges,
                               int& i, int& txRngSize) {
        textRanges.remove(i);
        txRngSize--;

        if (textRanges.size() <= 0)
            styleRanges.remove(styleName);
    };

    auto changeLengthRangeFunc = [&](const QString& styleName, QList<TextRange>& textRanges,
                                    TextRange& cRange, int& i, int& txRngSize) {
        cRange.addToLength(countToAdd);

        if (cRange.lenght <= 0) {
            textRanges.remove(i);
            txRngSize--;
            if (textRanges.size() <= 0)
                styleRanges.remove(styleName);
        }
    };

    QList<QString> keys = styleRanges.keys();
    int keysSize = keys.size();
    for (int i = 0; i < keysSize; i++) {
        QList<TextRange>& textRanges = styleRanges[keys[i]];
        int txRngSize = textRanges.size();

        int j = 0;
        for (; j < txRngSize; j++) {
            TextRange& cRange = textRanges[j];

            if (cRange.end() < endPosition)
                continue;
            else if (cRange.start > startPosition)
                break;

            int startDifference = cRange.start - endPosition;
            int endDifference = cRange.end() - startPosition;
            bool lengthChanged = false;

            //***********************
            if (startDifference >= 0 && endDifference > 0) {
                // Modificar solo START
                int offset = startPosition - cRange.start;
                cRange.addToLength(-offset);
                cRange.start = endPosition;
                lengthChanged = true;
            }
            else if (startDifference < 0 && endDifference <= 0) {
                // Modificar solo END
                int offset = cRange.end() - endPosition;
                cRange.addToLength(-offset);
                lengthChanged = true;
            }
            else if (startDifference >= 0 && endDifference <= 0) {
                // Eliminar todo
                removeRangeFunc(keys[i], textRanges, j, txRngSize);
                j--;
                continue;
            }
            else if (startDifference < 0 && endDifference > 0) {
                // MODIFICAR solo LENGTH
                changeLengthRangeFunc(keys[i], textRanges, cRange, j, txRngSize);
                lengthChanged = true;
            }
            if (cRange.lenght == 0) {
                removeRangeFunc(keys[i], textRanges, j, txRngSize);
                j--;
            }
            if (!lengthChanged)
                break;
            //***********************

//            if (cRange.start < startPosition && cRange.end() >= startPosition) {
//                changeLengthRangeFunc(keys[i], textRanges, cRange, j, txRngSize);
//                continue;
//            }
        }

        for (; j < txRngSize; j++) {
            TextRange& cRange = textRanges[j];

            cRange.addToStart(countToAdd);

            if (j > 0) {
                TextRange& otherRange = textRanges[j - 1];
                if (otherRange.end() == cRange.start) {
                    otherRange.setEnd(cRange.end());

                    removeRangeFunc(keys[i], textRanges, j, txRngSize);
                    j--;
                }
            }
        }
    }

    countToAdd = 0;
    startPosition = -1;

    debugTextRanges();
}



QDataStream &operator>>(QDataStream& in, TextRangesController controller) {
    in >> controller.styleRanges;
    return in;
}

QDataStream &operator<<(QDataStream& out, TextRangesController controller) {
    out << controller.styleRanges;
    return out;
}
