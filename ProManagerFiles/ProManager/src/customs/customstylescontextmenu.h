#ifndef CUSTOMSTYLESCONTEXTMENU_H
#define CUSTOMSTYLESCONTEXTMENU_H

#include "style.h"

#include <QMenu>

class CustomTextEdit;

class CustomStylesContextMenu : public QMenu
{
    Q_OBJECT

public:
    CustomStylesContextMenu(CustomTextEdit* textEdit, QWidget* parent);
    const QList<Style> &getStyles() const;
    void setAddStyleAction(QAction* action);

public slots:
    void customPopup();
    void addStyle();
    void setIconColorForNewStyle();
    void setFontForNewStyle();
    void setFontColorForNewStyle();

private:
    QList<Style> styles;
    CustomTextEdit* textEdit;

public:
    //QAction* addStyleAct;
};

#endif // CUSTOMSTYLESCONTEXTMENU_H
