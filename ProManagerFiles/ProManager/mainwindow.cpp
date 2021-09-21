#include "mainwindow.h"
#include "word/word.h"
#include "customtextedit.h"
#include "style.h"
#include "customaction.h"
#include "customcolorfontdialog.h"

#include <QtWidgets>

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
    const QString fileName = QFileDialog::getOpenFileName(this);
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

    if (isUntitled && textEdit->document()->isEmpty() && !isWindowModified()) {
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
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, QCoreApplication::applicationName(),tr("Cannot read file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    textEdit->setPlainText(in.readAll());
    QGuiApplication::restoreOverrideCursor();

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

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        if (!file.commit())
            errorMessage = tr("Cannot write file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    else
        errorMessage = tr("Cannot open file %1 for writing:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString());

    QGuiApplication::restoreOverrideCursor();

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
        curFile = tr("document%1.txt").arg(sequenceNumber++);
    else
        curFile = QFileInfo(fileName).canonicalFilePath();

    textEdit->document()->setModified(false);
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), curFile);
    if (fileName.isEmpty())
        return false;
    return saveFile(fileName);
}

void MainWindow::download()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Download"), QFileInfo(curFile).baseName() + ".docx");
    if (fileName.isEmpty())
        return;
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);

    // Open Word Application and create new document
    OfficeLib::Word::Word word;
    OfficeLib::Word::Document doc = word.createDocument();

    // Write text
    textEdit->writeToWord(doc, styles);

    // Save and close document and Word Application
    doc.saveAs(fileName);
    doc.close();
    word.quit();

    QGuiApplication::restoreOverrideCursor();
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

void MainWindow::customContextMenuRequested()
{
    customContextMenu->popup(QWidget::mapToGlobal(QPoint(textEdit->cursorRect().right() + 15,textEdit->cursorRect().bottom() + 60)));
}

void MainWindow::selectStyleIconColor()
{
    if (QToolButton* btn = qobject_cast<QToolButton*>(sender())) {
        QColor c = QColorDialog::getColor();
        btn->setStyleSheet("border: 1px solid black; background-color: " + c.name() + ";");
        styles.last().setIconColor(c);
    }
}

void MainWindow::selectStyleFont()
{
    if (QPushButton* btn = qobject_cast<QPushButton*>(sender())) {
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok);

        if (ok) {
//            qDebug() << font.family();
//            qDebug() << font.pointSize();
//            qDebug() << font.pointSizeF();
//            qDebug() << font.style();
            styles.last().setFont(font);
        }
    }
}

void MainWindow::selectStyleFontColor()
{
    if (QToolButton* btn = qobject_cast<QToolButton*>(sender())) {
        OfficeLib::Word::WdColor c = CustomColorFontDialog::getColor();
        QColor c2 = Qt::red;
        btn->setStyleSheet("border: 1px solid black; background-color: " + c2.name() + ";");
        styles.last().fontColor = c;
    }
}

void MainWindow::addStyleToCustomContextMenu()
{
    if (QPushButton* btn = qobject_cast<QPushButton*>(sender())) {
        if (QLineEdit* ledit = qobject_cast<QLineEdit*>(btn->parent()->children().at(2))) {
            for (Style &s : styles) {
                if (s.getName() == ledit->text())
                    return;
            }

            styles.last().setName(ledit->text());

            CustomAction* newAct = new CustomAction(styles.last());
            newAct->setEnabled(!textEdit->textCursor().selectedText().isEmpty());

            connect(newAct, &QAction::triggered, textEdit, &CustomTextEdit::setTextSelectedColor);
            connect(textEdit, &QTextEdit::copyAvailable, newAct, &QAction::setEnabled);

            customContextMenu->addAction(newAct);

            ledit->clear();
            btn->parent()->parent()->setProperty("visible", false);
            styles.append(Style());

            if (!textEdit->textCursor().selectedText().isEmpty())
                emit newAct->triggered();
        }
    }
}

void MainWindow::init()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    textEdit = new CustomTextEdit;
    setCentralWidget(textEdit);

    styles.append(Style());
    textEdit->styles = &styles;

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();
    createContextMenu();
    createCustomContextMenu();
    readSettings();
    connect(textEdit->document(), &QTextDocument::contentsChanged, this, &MainWindow::documentWasModified);
    setUnifiedTitleAndToolBarOnMac(true);

    textEdit->initTest();
//    textEdit->test("AL INICIO JEJE     ", styles);
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


    const QIcon downloadIcon = QIcon::fromTheme("document-download", QIcon(":/images/download.png"));
    downloadAct = new QAction(downloadIcon, tr("&Download"), this);
#ifdef Q_OS_WINDOWS
    downloadAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
#endif
    downloadAct->setStatusTip(tr("Download the document in the specified path"));
    connect(downloadAct, &QAction::triggered, this, &MainWindow::download);

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
    connect(cutAct, &QAction::triggered, textEdit, &QTextEdit::cut);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
    copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect(copyAct, &QAction::triggered, textEdit, &QTextEdit::copy);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
    pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    connect(pasteAct, &QAction::triggered, textEdit, &QTextEdit::paste);

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, &QTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
    connect(textEdit, &QTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD

    const QIcon resetSelectedTextFormatIcon = QIcon(":/images/reset-format.png");
    resetSelectedTextFormatAct = new QAction(resetSelectedTextFormatIcon, tr("&Reset Format"), this);
    resetSelectedTextFormatAct->setStatusTip(tr("Reset selected text format"));
    connect(resetSelectedTextFormatAct, &QAction::triggered, textEdit, &CustomTextEdit::resetSelectedTextFormat);
//    connect(resetSelectedTextFormatAct, &QAction::triggered, textEdit, &CustomTextEdit::test);
    resetSelectedTextFormatAct->setEnabled(false);
    connect(textEdit, &QTextEdit::copyAvailable, resetSelectedTextFormatAct, &QAction::setEnabled);

    const QIcon showStylesMenuIcon = QIcon(":/images/styles-menu.png");
    showStylesMenuAct = new QAction(showStylesMenuIcon, tr("Show Style Menu"), this);
    showStylesMenuAct->setShortcut(Qt::CTRL | Qt::Key_Space);
    showStylesMenuAct->setStatusTip(tr("Show a menu with styles options"));
    connect(showStylesMenuAct, &QAction::triggered, this, &MainWindow::customContextMenuRequested);
}

void MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
#ifdef Q_OS_WINDOWS
    fileMenu->addAction(downloadAct);
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

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
#ifndef QT_NO_CLIPBOARD
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
#endif
    editMenu->addAction(resetSelectedTextFormatAct);
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
    fileToolBar->addAction(downloadAct);

    QToolBar *editToolBar = addToolBar(tr("Edit"));
#ifndef QT_NO_CLIPBOARD
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
#endif
    editToolBar->addAction(resetSelectedTextFormatAct);
    editToolBar->addAction(showStylesMenuAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
    QDockWidget* createStyleDock = new QDockWidget(tr("Create New Style"), this);

    QWidget* widget = new QWidget(createStyleDock);
    QVBoxLayout* layout = new QVBoxLayout;

    QLabel* styleNameLabel = new QLabel(tr("Style Name"));
    QLineEdit* styleNameLEdit = new QLineEdit;

    QHBoxLayout* rectColorLayout = new QHBoxLayout;
    QLabel* rectColorLabel = new QLabel(tr("Style Color"));
    QToolButton* rectColorBtn = new QToolButton();

    QVBoxLayout* fontPropsLayout = new QVBoxLayout;
    QLabel* fontLabel = new QLabel(tr("Font Properties"));
    QHBoxLayout* fontOptionsLayout = new QHBoxLayout;
    QPushButton* fontBtn = new QPushButton(tr("Font"));
    QToolButton* rectFontColorBtn = new QToolButton();

    QPushButton* createStyleBtn = new QPushButton("Create");

    connect(rectColorBtn, &QToolButton::clicked, this, &MainWindow::selectStyleIconColor);
    connect(fontBtn, &QPushButton::clicked, this, &MainWindow::selectStyleFont);
    connect(rectFontColorBtn, &QPushButton::clicked, this, &MainWindow::selectStyleFontColor);
    connect(createStyleBtn, &QPushButton::clicked, this, &MainWindow::addStyleToCustomContextMenu);

    layout->addWidget(styleNameLabel);
    layout->addWidget(styleNameLEdit);

    rectColorLayout->addWidget(rectColorLabel);
    rectColorLayout->addWidget(rectColorBtn);
    layout->addLayout(rectColorLayout);

    layout->addStretch(1);

    fontPropsLayout->addWidget(fontLabel);
    fontOptionsLayout->addWidget(fontBtn);
    fontOptionsLayout->addWidget(rectFontColorBtn);
    fontPropsLayout->addLayout(fontOptionsLayout);
    layout->addLayout(fontPropsLayout);

    layout->addStretch(5);

    layout->addWidget(createStyleBtn);
    widget->setLayout(layout);

    createStyleDock->setWidget(widget);
    createStyleDock->setVisible(false);
    addStyleAct = createStyleDock->toggleViewAction();
    addDockWidget(Qt::RightDockWidgetArea, createStyleDock);
    viewMenu->addAction(createStyleDock->toggleViewAction());
}

void MainWindow::createContextMenu()
{
//    QAction* actionName;
//    textEdit->addAction(actionName);
//    textEdit->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createCustomContextMenu()
{
    customContextMenu = new QMenu(this);
    addStyleAct->setIcon(QIcon(":/images/plus.png"));
    customContextMenu->addAction(addStyleAct);
    customContextMenu->addSeparator();
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
    if (!textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret = QMessageBox::warning(this, QCoreApplication::applicationName(), tr("The document has been modified.\nDo you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch(ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}
