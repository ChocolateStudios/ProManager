#include "fileresourceimporterdock.h"

#include "docks/dockarguments.h"

#include "resources/controllers/resourcescontroller.h"
#include "resources/fileresource.h"

#include <QAction>
#include <QFileDialog>
#include <QGuiApplication>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QApplication>
#include <QClipboard>
#include <QStatusBar>

FileResourceImporterDock::FileResourceImporterDock(DockArguments args)
    : BaseDock(args.mainWin, tr("Files")), fileResourcesController(args.get<ResourcesController<FileResource>>(0)),
      row(0), column(0)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Widgets

    QWidget* widget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* headerLayout = new QHBoxLayout;
    QPushButton* importBtn = new QPushButton("Import");
    QPushButton* removeBtn = new QPushButton("Remove");
    headerLayout->addWidget(importBtn);
    headerLayout->addWidget(removeBtn);


    QScrollArea* scrollArea = new QScrollArea;
    scrollWidget = new QWidget;

    QRect rect = scrollWidget->geometry();
    rect.setHeight(0);
    rect.setWidth(188);
    scrollWidget->setGeometry(rect);

    filesImagesLayout = new QGridLayout;
    filesImagesLayout->setSpacing(6);

    QList<FileResource>& importedFiles = fileResourcesController->getResources();
    int size = importedFiles.size();
    for (int i = 0; i < size; i++) {
        addFileImageButton(importedFiles[i]);
    }

    QWidget* verticalSpacer = new QWidget;
    verticalSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Layout

    filesImagesLayout->addWidget(verticalSpacer);
    scrollWidget->setLayout(filesImagesLayout);
    scrollArea->setWidget(scrollWidget);

    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(scrollArea);

    widget->setLayout(mainLayout);
    setWidget(widget);

    args.mainWin->addDockWidget(Qt::RightDockWidgetArea, this);

    // Connections

    connect(importBtn, &QPushButton::clicked, this, &FileResourceImporterDock::importFile);
}

bool FileResourceImporterDock::isUniqueFile(const QString &fileName)
{
    QList<FileResource>& importedFiles = fileResourcesController->getResources();
    int size = importedFiles.size();
    for (int i = 0; i < size; i++) {
        if (importedFiles[i].getName() == fileName)
            return false;
    }
    return true;
}

void FileResourceImporterDock::addFileImageButton(const FileResource& currentFile)
{
    const QPixmap& fileIcon = QPixmap(":/images/word_icon.png");
    const QIcon icon(fileIcon);
    QPushButton* button = new QPushButton();
    button->setIcon(icon);
    button->setIconSize(QSize(70, 70));

    QAction* btnAct = new QAction(currentFile.getName());
    connect(btnAct, &QAction::triggered, this, [&]() {
        QAction* act = qobject_cast<QAction*>(sender());
        QApplication::clipboard()->setText(QString("<file src=\"%1\">").arg(act->text()));
        ((QMainWindow*)window())->statusBar()->showMessage(tr("File tag copied to clipboard"), 2000);
    });

    button->addAction(btnAct);
    connect(button, &QPushButton::clicked, this, [&]() {
        QPushButton* btn = qobject_cast<QPushButton*>(sender());
        emit btn->actions()[0]->triggered();
    });

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(button);
    vLayout->addWidget(new QLabel(currentFile.getName()));

    filesImagesLayout->addLayout(vLayout, (int)row, (int)column);

    if (column >= 1) {
        column = 0;
        row++;
    }
    else {
        column++;
        QRect rect = scrollWidget->geometry();
        rect.setHeight(rect.height() + 100);
        scrollWidget->setGeometry(rect);
    }
}

void FileResourceImporterDock::importFile()
{
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Import File"), QString(), tr("Word Document (*.docx);; Word 97-2003 Document (*.doc)"));

    if (!filePath.isEmpty()) {
        if (isUniqueFile(QFileInfo(filePath).fileName())) {

            //Add imported image to imagesLayout
            FileResource newResource = FileResource(QFileInfo(filePath).fileName(), filePath);
            fileResourcesController->appendResource(newResource);
            addFileImageButton(newResource);
        }
        else {
            QMessageBox::warning(this, QString(), tr("Cannot import files with the same name."));
        }
    }
}
