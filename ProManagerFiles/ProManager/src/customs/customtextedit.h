#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>

#include "extras/textanalizer.h"

namespace OfficeLib { namespace Word {
    class Document;
}}
class Style;
class CustomImage;
class QStandardItemModel;

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    CustomTextEdit();
    void setStyles(const QList<Style>* stls);
    void writeToWord(OfficeLib::Word::Document& wordDocument, const QList<Style> &styles, const QList<CustomImage>& importedImages);
    void setNavegationTexts(QStandardItemModel* model, const QStandardItemModel* otherModel);
    void updateBackgroundStyleColor(const QString& previousName, const Style& style);

    void saveData(QDataStream& out);
    void readData(QDataStream& in, const QList<Style> &styles);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

signals:
    void specialTextClicked();

public slots:
    void setTextSelectedColor();

private:
    void searchSpecialText();

private:
    TextAnalizer textAnalizer;
    const QList<Style>* styles;
    bool searchBtnPressed = false;
};

#endif // CUSTOMTEXTEDIT_H
