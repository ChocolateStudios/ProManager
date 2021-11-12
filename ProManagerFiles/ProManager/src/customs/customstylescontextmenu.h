#ifndef CUSTOMSTYLESCONTEXTMENU_H
#define CUSTOMSTYLESCONTEXTMENU_H

#include "extras/style.h"

#include <QMenu>

class CustomTextEdit;
class QPushButton;

class CustomStylesContextMenu : public QMenu
{
    Q_OBJECT

public:
    CustomStylesContextMenu(CustomTextEdit* textEdit, QWidget* parent);
    const QList<Style> &getStyles() const;
    void setAddStyleAction(QAction* action);
    void saveData(QDataStream &out);
    void readData(QDataStream &in);

    void updateStyleFromDock(QString styleName, QPushButton* btn, CustomTextEdit* textEdit);

public slots:
    void customPopup();
    void addStyleFromDock();

    void setIconColorForNewStyle();
    void setFontForNewStyle();
    void setFontColorForNewStyle();
    void setHeaderLevelForNewStyle();

    void setIconColorForUpdateStyle();
    void setFontForUpdateStyle();
    void setFontColorForUpdateStyle();
    void setHeaderLevelForUpdateStyle();

private:
    void addStyle();
    void updateStyle(Style& style, int index, CustomTextEdit* textEdit);

    QList<Style> styles;
    CustomTextEdit* textEdit;
};

#endif // CUSTOMSTYLESCONTEXTMENU_H
