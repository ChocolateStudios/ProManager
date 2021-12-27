#ifndef STYLESCONTEXTMENU_H
#define STYLESCONTEXTMENU_H

#include <QMenu>

class StyleResource;
class BaseEditor;
class QMainWindow;
class TextAnalizer;
class StyleResourcesController;

class StylesContextMenu : public QMenu
{
public:
    StylesContextMenu(QMainWindow* win, TextAnalizer* analizer, StyleResourcesController* stylesController);

    void addStyle(const StyleResource& style);
    void updateStyle(const unsigned int& index, const StyleResource& newStyle);
    void removeStyle(const unsigned int& index);
    void clearStyles();

    void setActiveEditor(BaseEditor* editor);

public slots:
    void popupInSelection();

private:
    QMainWindow* mainWin;
    TextAnalizer* textAnalizer;
    BaseEditor* activeEditor = nullptr;
    StyleResourcesController* styleResourcesController;
    int stylesCount = 0;
    const int stylesStartIndex = 2;
};

#endif // STYLESCONTEXTMENU_H
