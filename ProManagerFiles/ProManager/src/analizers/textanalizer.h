#ifndef TEXTANALIZER_H
#define TEXTANALIZER_H

#include <QTextCursor>

struct TextRange;

class QStandardItemModel;
class BaseEditor;
class StyleResourcesController;
class StyleResource;
class QStandardItem;
class DockController;

class TextAnalizer : public QObject
{
    Q_OBJECT
public:
    TextAnalizer(StyleResourcesController* styleResourcesController);

    void colorSelectedText(const StyleResource &newStyle);
    void updateStyleColorInEditors(const QString& oldStyleName, const StyleResource& newStyle);
    QStandardItemModel* getNavegationModel(const QStandardItemModel* styleHierarchyModel);

    void setActiveEditor(BaseEditor* editor);
    inline void setDockController(DockController* dockController) { this->dockController = dockController; }

public slots:
    void searchTagInEditor();
    void openDockByTagType();
    void stopSearchingTagText();
    void onColorSelectedText(const StyleResource &newStyle);

private:
    void appendNextNavegationLevel(const QStandardItemModel *otherModel, QStandardItem* lastLevel, int levelIndex, int lastLevelItemIndex, QTextCursor& textCursor, int lastEndIndex, int maxStringLength);

    void overrideCursor();
    void restoreOverridedCursor();
    void removeUnderlinedTagText();

private:
    StyleResourcesController* styleResourcesController;
    QTextCursor tagTextSelectedCursor;

    BaseEditor* activeEditor;
    DockController* dockController;
    bool cursorOverrided = false;
};

#endif // TEXTANALIZER_H
