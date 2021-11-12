#include "filesdock.h"

#include "customs/customfilepushbutton.h"
#include "customs/customfile.h"

#include <QFileDialog>
#include <QGuiApplication>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

FilesDock::FilesDock(QMainWindow *mainWin, QList<CustomFile>* importedFls)
    : BaseDock(tr("Files"), mainWin), row(0), column(0), importedFiles(importedFls)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* widget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* headerLayout = new QHBoxLayout;
    QPushButton* importBtn = new QPushButton("Import");
    QPushButton* removeBtn = new QPushButton("Remove");
    headerLayout->addWidget(importBtn);
    headerLayout->addWidget(removeBtn);

    connect(importBtn, &QPushButton::clicked, this, &FilesDock::importFile);

    QScrollArea* scrollArea = new QScrollArea;
    scrollWidget = new QWidget;

    QRect rect = scrollWidget->geometry();
    rect.setHeight(0);
    rect.setWidth(188);
    scrollWidget->setGeometry(rect);

    filesImagesLayout = new QGridLayout;
    filesImagesLayout->setSpacing(6);

    for(const CustomFile& cFile : *importedFiles) {
        addFileImageButton(cFile, QPixmap(":/images/word_icon.png"));
    }

    QWidget* verticalSpacer = new QWidget;
    verticalSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    filesImagesLayout->addWidget(verticalSpacer);
    scrollWidget->setLayout(filesImagesLayout);
    scrollArea->setWidget(scrollWidget);

    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(scrollArea);

    widget->setLayout(mainLayout);
    setWidget(widget);

    mainWin->addDockWidget(Qt::RightDockWidgetArea, this);
}

bool FilesDock::isUniqueFile(const QString &fileName)
{
    for (const CustomFile& c : *importedFiles) {
        if (c.name == fileName)
            return false;
    }
    return true;
}

void FilesDock::addFileImageButton(const CustomFile& currentFile, const QPixmap& fileIcon)
{
    const QIcon icon(fileIcon);
    CustomFilePushButton* button = new CustomFilePushButton(currentFile.name, (QMainWindow*)window());
    button->setIcon(icon);
    button->setIconSize(QSize(70, 70));


    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(button);
    vLayout->addWidget(new QLabel(currentFile.name));

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

void FilesDock::importFile()
{
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Import File"), QString(), tr("Word Document (*.docx);; Word 97-2003 Document (*.doc)"));

    if (!filePath.isEmpty()) {
        if (isUniqueFile(QFileInfo(filePath).fileName())) {
            QPixmap pixmap(":/images/word_icon.png");

            //Add imported image to imagesLayout
            CustomFile cImg(filePath);
            importedFiles->append(cImg);
            addFileImageButton(cImg, pixmap);
        }
        else {
            QMessageBox::warning(this, QString(), tr("Cannot import files with the same name."));
        }
    }
}
