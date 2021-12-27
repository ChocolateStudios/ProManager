#include "mainwindow.h"

#include "maintabwidget.h"

#include "docks/dockcontroller.h"

#include "resources/manager/resourcesmanager.h"

#include "datastream/readfilestream.h"
#include "datastream/savefilestream.h"

#include "exporter/exporter.h"

#include <QtWidgets>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    init();
    setCurrentFile(QString());
}

MainWindow::MainWindow(const QString &fileName)
{
    init();
    loadFile(fileName);
}

void MainWindow::tile(const QMainWindow *previous)
{
    if (!previous)
        return;
    int topFrameWidth = previous->geometry().top() - previous->pos().y();
    if (!topFrameWidth)
        topFrameWidth = 40;
    const QPoint pos = previous->pos() + 2 * QPoint(topFrameWidth, topFrameWidth);
    if (screen()->availableGeometry().contains(rect().bottomRight() + pos))
        move(pos);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    }
    else
        event->ignore();
}

void MainWindow::newFile()
{
    MainWindow* other = new MainWindow;
    other->tile(this);
    other->show();
}

void MainWindow::open()
{
    QString selectedFilter = "ProManager files (*.choco)";
    const QString fileName = QFileDialog::getOpenFileName(this, "Open", QString(), "All files (*.*);; ProManager files (*.choco);; Text Document (*.txt)", &selectedFilter);
    if (!fileName.isEmpty())
        openFile(fileName);
}

void MainWindow::openFile(const QString &fileName)
{
    MainWindow* existing = findMainWindow(fileName);
    if (existing) {
        existing->show();
        existing->raise();
        existing->activateWindow();
        return;
    }

    if (isUntitled && mainTabWidget->allEditorsEmpty() && !isWindowModified()) {
        loadFile(fileName);
        return;
    }

    MainWindow* other = new MainWindow(fileName);
    if (other->isUntitled) {
        delete other;
        return;
    }

    other->tile(this);
    other->show();
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, QCoreApplication::applicationName(),tr("Cannot read file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    ReadFileStream readFile(file, fileName, this);

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::setRecentFilesVisible(const bool &visible)
{
    recentFileSubMenuAct->setVisible(visible);
    recentFileSeparator->setVisible(visible);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QString errorMessage;

    QSaveFile file(fileName);
    SaveFileStream saveFile(file, fileName, errorMessage, this);

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, QCoreApplication::applicationName(), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    static int sequenceNumber = 1;

    isUntitled = fileName.isEmpty();
    if (isUntitled)
        curFile = tr("document%1.choco").arg(sequenceNumber++);
    else
        curFile = QFileInfo(fileName).canonicalFilePath();

    mainTabWidget->setModifiersStateToEditors(false);
    setWindowModified(false);

    if (!isUntitled && windowFilePath() != curFile)
        MainWindow::prependToRecentFiles(curFile);

    setWindowFilePath(curFile);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

MainWindow *MainWindow::findMainWindow(const QString &fileName) const
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    const QList<QWidget*> topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget* widget : topLevelWidgets) {
        MainWindow *mainWin = qobject_cast<MainWindow*>(widget);
        if (mainWin && mainWin->curFile == canonicalFilePath)
            return mainWin;
    }

    return nullptr;
}

static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
static inline QString fileKey() { return QStringLiteral("file"); }

static QStringList readRecentFiles(QSettings& settings)
{
    QStringList result;
    const int count = settings.beginReadArray(recentFilesKey());
    for (int i = 0; i < count; i++) {
        settings.setArrayIndex(i);
        result.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    return result;
}

static void writeRecentFiles(const QStringList& files, QSettings& settings)
{
    const int count = files.size();
    settings.beginWriteArray(recentFilesKey());
    for (int i = 0; i < count; i++) {
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), files.at(i));
    }
    settings.endArray();
}

bool MainWindow::hasRecentFiles()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = settings.beginReadArray(recentFilesKey());
    settings.endArray();
    return count > 0;
}

void MainWindow::prependToRecentFiles(const QString &fileName)
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList oldRecentFiles = readRecentFiles(settings);
    QStringList recentFiles = oldRecentFiles;
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    if (oldRecentFiles != recentFiles)
        writeRecentFiles(recentFiles, settings);

    setRecentFilesVisible(!recentFiles.isEmpty());
}

bool MainWindow::save()
{
    return isUntitled ? saveAs() : saveFile(curFile);
}

bool MainWindow::saveAs()
{
    QString selectedFilter = "ProManager files (*.choco)";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), curFile, "All files (*.*);; ProManager files (*.choco);; Text Document (*.txt)", &selectedFilter);
    if (fileName.isEmpty())
        return false;
    return saveFile(fileName);
}

void MainWindow::exportTo()
{
    Exporter exporter(this);
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(int(MaxRecentFiles), recentFiles.size());
    int i = 0;
    for (; i < count; i++) {
        const QString fileName = MainWindow::strippedName(recentFiles.at(i));
        recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
        recentFileActs[i]->setData(recentFiles.at(i));
        recentFileActs[i]->setVisible(true);
    }
    for (; i < MaxRecentFiles; i++)
        recentFileActs[i]->setVisible(false);
}

void MainWindow::openRecentFile()
{
    if (const QAction* action = qobject_cast<QAction*>(sender()))
        openFile(action->data().toString());
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About ") + QCoreApplication::applicationName(), tr("The <b>") + QCoreApplication::applicationName() + tr("</b> was made by Chocolate Studios."));
}

void MainWindow::documentWasModified()
{
    setWindowModified(true);
}

void MainWindow::init()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;

    resourcesManager = new ResourcesManager();
    mainTabWidget = new MainTabWidget(this);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    mainTabWidget->connectMainWinActions();

    dockController = new DockController(this);
    mainTabWidget->setOtherParameters();

    mainTabWidget->stylesContextMenu->addAction(dockController->getAddStyleAction());
    mainTabWidget->stylesContextMenu->addSeparator();
    mainTabWidget->stylesContextMenu->addStyle(StyleResource(tr("Comment"), Qt::white));

    setCentralWidget(mainTabWidget);

    readSettings();
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::createActions()
{
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    saveAsAct = new QAction(saveAsIcon, tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);


    const QIcon exportIcon = QIcon::fromTheme("document-download", QIcon(":/images/download.png"));
    exportAct = new QAction(exportIcon, tr("&Export"), this);
#ifdef Q_OS_WINDOWS
    exportAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
#endif
    exportAct->setStatusTip(tr("Export the documents in the specified path"));
    connect(exportAct, &QAction::triggered, this, &MainWindow::exportTo);

    const QIcon closeIcon = QIcon::fromTheme("application-close");
    closeAct = new QAction(closeIcon, tr("Close"), this);
    closeAct->setShortcut(tr("Ctrl+W"));
    closeAct->setStatusTip(tr("Close this window"));
    connect(closeAct, &QAction::triggered, this, &QWidget::close);

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    exitAct = new QAction(exitIcon, tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, qApp, &QApplication::quit);


#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
    cutAct = new QAction(cutIcon, tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
    copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
    pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));

//    cutAct->setEnabled(false);
//    copyAct->setEnabled(false);
    cutAct->setEnabled(true);
    copyAct->setEnabled(true);
#endif // !QT_NO_CLIPBOARD

    const QIcon showStylesMenuIcon = QIcon(":/images/styles-menu.png");
    showStylesMenuAct = new QAction(showStylesMenuIcon, tr("Show Style Menu"), this);
    showStylesMenuAct->setShortcut(Qt::CTRL | Qt::Key_Space);
    showStylesMenuAct->setStatusTip(tr("Show a menu with styles options"));
    connect(showStylesMenuAct, &QAction::triggered, mainTabWidget->stylesContextMenu, &StylesContextMenu::popupInSelection);

    const QIcon insertTableIcon = QIcon(":/images/table.png");
    insertTableAct = new QAction(insertTableIcon, tr("New Table"), this);
    insertTableAct->setShortcut(Qt::CTRL | Qt::Key_T);
    insertTableAct->setStatusTip(tr("Insert a table"));

    const QIcon insertColumnRightInTableIcon = QIcon(":/images/insertRightColumn.png");
    insertColumnRightInTableAct = new QAction(insertColumnRightInTableIcon, tr("Insert Column Right"), this);
    insertColumnRightInTableAct->setStatusTip(tr("Insert a new column to the right of the current column"));

    const QIcon insertColumnLeftInTableIcon = QIcon(":/images/insertLeftColumn.png");
    insertColumnLeftInTableAct = new QAction(insertColumnLeftInTableIcon, tr("Insert Column Left"), this);
    insertColumnLeftInTableAct->setStatusTip(tr("Insert a new column to the left of the current column"));

    const QIcon insertRowDownInTableIcon = QIcon(":/images/insertRowDown.png");
    insertRowDownInTableAct = new QAction(insertRowDownInTableIcon, tr("Insert Row Below"), this);
    insertRowDownInTableAct->setStatusTip(tr("Insert a new row below of the current row"));

    const QIcon insertRowUpInTableIcon = QIcon(":/images/insertRowUp.png");
    insertRowUpInTableAct = new QAction(insertRowUpInTableIcon, tr("Insert Row Above"), this);
    insertRowUpInTableAct->setStatusTip(tr("Insert a new row above of the current row"));

    const QIcon removeTableIcon = QIcon(":/images/removeTable.png");
    removeTableAct = new QAction(removeTableIcon, tr("Remove Table"), this);
    removeTableAct->setStatusTip(tr("Remove current table"));

    const QIcon removeCurrentColumnInTableIcon = QIcon(":/images/removeColumn.png");
    removeCurrentColumnInTableAct = new QAction(removeCurrentColumnInTableIcon, tr("Remove Column"), this);
    removeCurrentColumnInTableAct->setStatusTip(tr("Remove current column"));

    const QIcon removeCurrentRowInTableIcon = QIcon(":/images/removeRow.png");
    removeCurrentRowInTableAct = new QAction(removeCurrentRowInTableIcon, tr("Remove Row"), this);
    removeCurrentRowInTableAct->setStatusTip(tr("Remove current row"));

    showCharactersIndex = new QAction(tr("Show characters index"), this);
    showCharactersIndex->setStatusTip(tr("Show characters index"));
    connect(showCharactersIndex, &QAction::triggered, mainTabWidget, &MainTabWidget::showCharactersIndexInActiveEditor);
}

void MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
#ifdef Q_OS_WINDOWS
    fileMenu->addAction(exportAct);
#endif

    fileMenu->addSeparator();


    QMenu *recentMenu = fileMenu->addMenu(tr("Recent..."));
    connect(recentMenu, &QMenu::aboutToShow, this, &MainWindow::updateRecentFileActions);
    recentFileSubMenuAct = recentMenu->menuAction();

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = recentMenu->addAction(QString(), this, &MainWindow::openRecentFile);
        recentFileActs[i]->setVisible(false);
    }

    recentFileSeparator = fileMenu->addSeparator();

    setRecentFilesVisible(MainWindow::hasRecentFiles());

    fileMenu->addAction(closeAct);
    fileMenu->addAction(exitAct);

    insertMenu = menuBar()->addMenu(tr("Insert"));
    insertMenu->addAction(insertTableAct);
    insertMenu->addAction(insertColumnRightInTableAct);
    insertMenu->addAction(insertColumnLeftInTableAct);
    insertMenu->addAction(insertRowDownInTableAct);
    insertMenu->addAction(insertRowUpInTableAct);
    insertMenu->addAction(removeTableAct);
    insertMenu->addAction(removeCurrentColumnInTableAct);
    insertMenu->addAction(removeCurrentRowInTableAct);


    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
#ifndef QT_NO_CLIPBOARD
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
#endif
    editMenu->addAction(showStylesMenuAct);


    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();


    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));


    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::createToolBars()
{
    QToolBar *fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(exportAct);

    QToolBar *editToolBar = addToolBar(tr("Edit"));
#ifndef QT_NO_CLIPBOARD
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
#endif
    editToolBar->addAction(showStylesMenuAct);
    editToolBar->addAction(showCharactersIndex);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

bool MainWindow::maybeSave()
{
    switch (mainTabWidget->maybeSave()) {
    case 1:
        return save();
    case 2:
        return false;
    }
    return true;
}
