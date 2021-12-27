#include "maintabwidget.h"

#include "mainwindow.h"

#include "resources/manager/resourcesmanager.h"

#include "datastream/readfilestream.h"

#include "analizers/textanalizer.h"

#include "menus/stylescontextmenu.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QTabBar>
#include <QToolButton>
#include <QMessageBox>
#include <QCoreApplication>
#include <QFileInfo>
#include <QStyle>

MainTabWidget::MainTabWidget(MainWindow* win)
    : mainWin(win)
{
    createInitialTab();

    StyleResourcesController* styleResourcesController = (StyleResourcesController*)mainWin->resourcesManager->getResourcesController<StyleResource>(0);

    textAnalizer = new TextAnalizer(styleResourcesController);
    stylesContextMenu = new StylesContextMenu(mainWin, textAnalizer, styleResourcesController);

    connect(this, &QTabWidget::currentChanged, this, &MainTabWidget::changeActiveEditor);
    connect(this, &QTabWidget::tabCloseRequested, this, &MainTabWidget::removeTabEditor);

    setTabsClosable(true);

    tabBar()->setTabButton(0, QTabBar::ButtonPosition::RightSide, 0);
}

short MainTabWidget::maybeSave()
{
    int size = textDocumentEditors.size();

    for (int i = 0; i < size; i++) {
        if (textDocumentEditors[i]->document()->isModified()) {
            const QMessageBox::StandardButton ret = QMessageBox::warning(this, QCoreApplication::applicationName(), tr("The document %1 has been modified.\nDo you want to save your changes?").arg(textDocumentEditors[i]->getEditorName()), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            switch(ret) {
            case QMessageBox::Save:
                return 1;
            case QMessageBox::Cancel:
                return 2;
            default:
                break;
            }
        }
    }
    return 3;
}

void MainTabWidget::connectMainWinActions()
{
#ifndef QT_NO_CLIPBOARD
    connect(mainWin->cutAct, &QAction::triggered, this, [&]() {
        if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
            textDocumentEditor->cut();
        }
    });
    connect(mainWin->copyAct, &QAction::triggered, this, [&]() {
        if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
            textDocumentEditor->copy();
        }
    });
    connect(mainWin->pasteAct, &QAction::triggered, this, [&]() {
        if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
            textDocumentEditor->paste();
        }
    });

//    connect(textEdit, &QTextEdit::copyAvailable, mainWin->cutAct, &QAction::setEnabled);
//    connect(textEdit, &QTextEdit::copyAvailable, mainWin->copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD

    connect(mainWin->insertTableAct, &QAction::triggered, this, [&]() {
        if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
            textDocumentEditor->insertTable();
        }
    });
    connect(mainWin->insertColumnRightInTableAct, &QAction::triggered, this, [&]() {
        if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
            textDocumentEditor->insertColumnRightInTable();
        }
    });
    connect(mainWin->insertColumnLeftInTableAct, &QAction::triggered, this, [&]() {
        if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
            textDocumentEditor->insertColumnLeftInTable();
        }
    });
    connect(mainWin->insertRowDownInTableAct, &QAction::triggered, this, [&]() {
        if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
            textDocumentEditor->insertRowDownInTable();
        }
    });
    connect(mainWin->insertRowUpInTableAct, &QAction::triggered, this, [&]() {
        if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
            textDocumentEditor->insertRowUpInTable();
        }
    });
    connect(mainWin->removeTableAct, &QAction::triggered, this, [&]() {
        if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
            textDocumentEditor->removeTable();
        }
    });
    connect(mainWin->removeCurrentColumnInTableAct, &QAction::triggered, this, [&]() {
        if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
            textDocumentEditor->removeCurrentColumnInTable();
        }
    });
    connect(mainWin->removeCurrentRowInTableAct, &QAction::triggered, this, [&]() {
        if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
            textDocumentEditor->removeCurrentRowInTable();
        }
    });
}

void MainTabWidget::setOtherParameters()
{
    textAnalizer->setDockController(mainWin->dockController);
}

void MainTabWidget::addTabWithTextDocumentEditor(TextDocumentEditor *newDocEditor)
{
    int lastIndex = count() - 1;

    connect(newDocEditor->document(), &QTextDocument::contentsChanged, mainWin, &MainWindow::documentWasModified);
    connect(newDocEditor, &TextDocumentEditor::mouseMoved, textAnalizer, &TextAnalizer::searchTagInEditor);
    connect(newDocEditor, &TextDocumentEditor::tagSearcherBtnReleasedText, textAnalizer, &TextAnalizer::stopSearchingTagText);
    connect(newDocEditor, &TextDocumentEditor::leftMouseBtnClicked, textAnalizer, &TextAnalizer::openDockByTagType);
    connect(newDocEditor, &TextDocumentEditor::colorSelectedText, textAnalizer, &TextAnalizer::onColorSelectedText);

    textDocumentEditors.append(newDocEditor);

    QString tabName = QString("document%1.xd").arg(textDocumentEditors.size());
    newDocEditor->updateEditorName(tabName);

    insertTab(lastIndex, newDocEditor, tabName);
    setCurrentIndex(lastIndex);

    tabNames.append(tabName);
}

void MainTabWidget::addTabWithSpreadsheetEditor(SpreadsheetEditor *newSpreadsheetEditor)
{
    int lastIndex = count() - 1;

    spreadsheetEditors.append(newSpreadsheetEditor);

    QString tabName = QString("sheet%1.uwu").arg(spreadsheetEditors.size());
    newSpreadsheetEditor->updateEditorName(tabName);

    insertTab(lastIndex, newSpreadsheetEditor, tabName);
    setCurrentIndex(lastIndex);

    tabNames.append(tabName);
}

void MainTabWidget::setModifiersStateToEditors(bool modified)
{
    int size = textDocumentEditors.size();

    for (int i = 0; i < size; i++)
        textDocumentEditors[i]->document()->setModified(modified);
}

bool MainTabWidget::allEditorsEmpty()
{
    int size = textDocumentEditors.size();

    for (int i = 0; i < size; i++)
        if (!textDocumentEditors[i]->document()->isEmpty())
            return false;

    return true;
}

void MainTabWidget::updateActiveEditor(BaseEditor *active)
{
    activeEditor = active;
    stylesContextMenu->setActiveEditor(active);
    textAnalizer->setActiveEditor(active);
}

void MainTabWidget::updateTextRangesOfAllEditors()
{
    int size = textDocumentEditors.size();

    for (int i = 0; i < size; i++)
        textDocumentEditors[i]->updateTextRanges();
}

BaseEditor *MainTabWidget::getEditorByName(const QString &editorName)
{
    QString tabSuffix = QFileInfo(editorName).suffix();

    if (tabSuffix == "xd") {
        int size = textDocumentEditors.size();
        for (int i = 0; i < size; i++) {
            if (editorName == textDocumentEditors[i]->getEditorName())
                return textDocumentEditors[i];
        }
    }

    else if (tabSuffix == "uwu") {
        int size = spreadsheetEditors.size();
        for (int i = 0; i < size; i++) {
            if (editorName == spreadsheetEditors[i]->getEditorName())
                return spreadsheetEditors[i];
        }
    }

    return nullptr;
}

void MainTabWidget::readEditorsContent(QDataStream &in)
{
    int size;
    in >> size;

    for (int i = 0; i < size; i++) {
        TextDocumentEditor* newTextDocEditor = new TextDocumentEditor((StyleResourcesController*)mainWin->resourcesManager->getResourcesController<StyleResource>(0));
        in >> newTextDocEditor;
        addTabWithTextDocumentEditor(newTextDocEditor);
    }

    in >> size;
    for (int i = 0; i < size; i++) {
        SpreadsheetEditor* newSpreadsheetEditor = new SpreadsheetEditor();
        in >> newSpreadsheetEditor;
        addTabWithSpreadsheetEditor(newSpreadsheetEditor);
    }

    size = textDocumentEditors.size();
    for (int i = 0; i < size; i++) {
        QTextCursor cur(textDocumentEditors[i]->textCursor());
        textDocumentEditors[i]->textRangesController.paintTextRanges(cur, [&](const QString& styleName) {
            return ((StyleResourcesController*)(mainWin->resourcesManager->getResourcesController<StyleResource>(0)))->getStyleByName(styleName);
        });
    }
}

void MainTabWidget::saveEditorsContent(QDataStream &out)
{
    int size = textDocumentEditors.size();
    out << size;

    for (int i = 0; i < size; i++) {
        out << textDocumentEditors[i];
    }

    size = spreadsheetEditors.size();
    out << size;
    for (int i = 0; i < size; i++) {
        out << spreadsheetEditors[i];
    }
}

void MainTabWidget::addTabTextDocumentEditor()
{
    TextDocumentEditor* newDocEditor = new TextDocumentEditor((StyleResourcesController*)mainWin->resourcesManager->getResourcesController<StyleResource>(0));
    addTabWithTextDocumentEditor(newDocEditor);
}

void MainTabWidget::addTabSpreadsheetEditor()
{
    SpreadsheetEditor* newSpreadsheetEditor = new SpreadsheetEditor();
    addTabWithSpreadsheetEditor(newSpreadsheetEditor);
}

void MainTabWidget::changeActiveEditor(int index)
{
    if (index != count() - 1) {
        QString tabName = tabText(index);

        updateActiveEditor(getEditorByName(tabName));
    }

    else{
        updateActiveEditor(nullptr);
    }
}

void MainTabWidget::removeTabEditor(int index)
{
    QString tabName = tabText(index);
    QString tabSuffix = QFileInfo(tabName).suffix();

    removeTab(index);

    if (tabSuffix == "xd") {
        int size = textDocumentEditors.size();

        for (int i = 0; i < size; i++) {
            if (textDocumentEditors[i]->getEditorName() == tabName) {
                tabNames.remove(index);
                delete textDocumentEditors[i];
                textDocumentEditors.remove(i);
                break;
            }
        }

    }
    else if (tabSuffix == "uwu") {
        int size = spreadsheetEditors.size();

        for (int i = 0; i < size; i++) {
            if (spreadsheetEditors[i]->getEditorName() == tabName) {
                tabNames.remove(index);
                delete spreadsheetEditors[i];
                spreadsheetEditors.remove(i);
                break;
            }
        }
    }
}

void MainTabWidget::showCharactersIndexInActiveEditor()
{
    if (TextDocumentEditor* textDocumentEditor = dynamic_cast<TextDocumentEditor*>(activeEditor)) {
        textDocumentEditor->showCharactersIndex();
    }
}

void MainTabWidget::createInitialTab()
{
    QWidget* firstTab = new QWidget;

    QVBoxLayout* layout = new QVBoxLayout;

    QPushButton* createWordEditorBtn = new QPushButton(tr("Text Document Editor"));
    QPushButton* createSpreadsheetEditorBtn = new QPushButton(tr("Spreadsheet Editor"));

    connect(createWordEditorBtn, &QPushButton::clicked, this, &MainTabWidget::addTabTextDocumentEditor);
    connect(createSpreadsheetEditorBtn, &QPushButton::clicked, this, &MainTabWidget::addTabSpreadsheetEditor);

    layout->addWidget(createWordEditorBtn);
    layout->addWidget(createSpreadsheetEditorBtn);

    firstTab->setLayout(layout);

    addTab(firstTab, "+");
}
