#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CustomTextEdit;
class QAction;
class QLineEdit;
class Style;
class CustomStylesContextMenu;

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

private:
    enum { MaxRecentFiles = 5, MagicNumber = 0x4F94C8D3 };

    void init();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void initializeDockWindows();
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
    QAction *toggleAddStyleDockAct;

    QMenu* viewMenu;

    CustomStylesContextMenu* stylesContextMenu;

    QString curFile;
    bool isUntitled;
};
#endif // MAINWINDOW_H
