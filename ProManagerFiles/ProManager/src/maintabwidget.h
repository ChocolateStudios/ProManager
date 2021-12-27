#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H

#include "editors/textdocumenteditor.h"
#include "editors/spreadsheeteditor.h"

#include <QTabWidget>

class MainWindow;
class ReadFileStream;
class TextAnalizer;
class StylesContextMenu;

class MainTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    MainTabWidget(MainWindow* win);

    short maybeSave();
    void connectMainWinActions();
    void setOtherParameters();

    //Editors
    void addTabWithTextDocumentEditor(TextDocumentEditor* editor);
    void addTabWithSpreadsheetEditor(SpreadsheetEditor* editor);
    void setModifiersStateToEditors(bool modified);
    bool allEditorsEmpty();
    void updateActiveEditor(BaseEditor* active);
    void updateTextRangesOfAllEditors();
    BaseEditor* getEditorByName(const QString& editorName);

    //Save and Read Data
    void readEditorsContent(QDataStream& in);
    void saveEditorsContent(QDataStream& out);

public slots:
    void addTabTextDocumentEditor();
    void addTabSpreadsheetEditor();
    void changeActiveEditor(int index);
    void removeTabEditor(int index);

    void showCharactersIndexInActiveEditor();

private:
    void createInitialTab();

private:
    QList<TextDocumentEditor*> textDocumentEditors;
    QList<SpreadsheetEditor*> spreadsheetEditors;

    bool ctrlBtnPressed = false;
    MainWindow* mainWin = nullptr;

public:
    QStringList tabNames;   // is used by FileExportPropertiesDock
    TextAnalizer* textAnalizer = nullptr;
    StylesContextMenu* stylesContextMenu = nullptr;
    BaseEditor* activeEditor = nullptr;
};

#endif // MAINTABWIDGET_H
