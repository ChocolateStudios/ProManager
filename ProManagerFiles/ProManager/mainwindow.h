#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CustomTextEdit;
class QAction;
class QLineEdit;
class Style;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    explicit MainWindow(const QString& fileName);
    void tile(const QMainWindow* previous);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void download();
    void updateRecentFileActions();
    void openRecentFile();
    void about();
    void documentWasModified();
    void customContextMenuRequested();
    void selectStyleIconColor();
    void selectStyleFont();
    void selectStyleFontColor();
    void addStyleToCustomContextMenu();

private:
    enum { MaxRecentFiles = 5 };

    void init();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void createContextMenu();
    void createCustomContextMenu();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void openFile(const QString& fileName);
    void loadFile(const QString& fileName);
    static bool hasRecentFiles();
    void prependToRecentFiles(const QString& fileName);
    void setRecentFilesVisible(const bool& visible);
    bool saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);
    static QString strippedName(const QString& fullFileName);
    MainWindow *findMainWindow(const QString& fileName) const;

    CustomTextEdit *textEdit;

    QAction* recentFileActs[MaxRecentFiles];
    QAction* recentFileSeparator;
    QAction* recentFileSubMenuAct;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *downloadAct;
    QAction *closeAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *resetSelectedTextFormatAct;
    QAction *showStylesMenuAct;
    QAction *addStyleAct;

    QMenu* customContextMenu;
    QMenu* viewMenu;

    QList<Style> styles;

    QString curFile;
    bool isUntitled;
};
#endif // MAINWINDOW_H
