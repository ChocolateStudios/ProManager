#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTextEdit;
class QAction;

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
    enum { MaxRecentFiles = 5 };

    void init();
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void openFile(const QString& fileName);
    void loadFile(const QString& fileName);
    static bool hasRecentFiles();
    void prependToRecentFiles(const QString& fileName);
    void setRecentFilesVisible(bool visible);
    bool saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);
    static QString strippedName(const QString& fullFileName);
    MainWindow *findMainWindow(const QString& fileName) const;

    QTextEdit *textEdit;

    QAction* recentFileActs[MaxRecentFiles];
    QAction* recentFileSeparator;
    QAction* recentFileSubMenuAct;

    QString curFile;
    bool isUntitled;
};
#endif // MAINWINDOW_H
