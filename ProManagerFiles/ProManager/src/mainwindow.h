#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class DockController;
class MainTabWidget;
class ResourcesManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class ReadFileStream;
    friend class SaveFileStream;
    friend class MainTabWidget;
    friend class DockController;
    friend class Exporter;
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
    void exportTo();
    void updateRecentFileActions();
    void openRecentFile();
    void about();

public slots:
    void documentWasModified();

private:
    enum { MaxRecentFiles = 5, MagicNumber = 0x4F94C8D3 };

    void init();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
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

    QAction* recentFileActs[MaxRecentFiles];
    QAction* recentFileSeparator;
    QAction* recentFileSubMenuAct;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exportAct;
    QAction *closeAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *showStylesMenuAct;

    QAction *insertTableAct;
    QAction *insertColumnRightInTableAct;
    QAction *insertColumnLeftInTableAct;
    QAction *insertRowDownInTableAct;
    QAction *insertRowUpInTableAct;
    QAction *removeTableAct;
    QAction *removeCurrentColumnInTableAct;
    QAction *removeCurrentRowInTableAct;

    QAction *showCharactersIndex;

    QMenu* insertMenu;
    QMenu* viewMenu;

    QString curFile;
    bool isUntitled;

public:
    MainTabWidget* mainTabWidget;
    DockController* dockController;
    ResourcesManager* resourcesManager;
};
#endif // MAINWINDOW_H
