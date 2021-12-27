#include "imageresourceimporterdock.h"

#include "docks/dockarguments.h"

#include "resources/controllers/resourcescontroller.h"

#include "resources/imageresource.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QGuiApplication>
#include <QMessageBox>
#include <QList>
#include <QApplication>
#include <QClipboard>
#include <QStatusBar>
#include <QAction>

ImageResourceImporterDock::ImageResourceImporterDock(DockArguments args)
    :BaseDock(args.mainWin, tr("Images")), imageResourcesController(args.get<ResourcesController<ImageResource>>(0)),
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

    imagesLayout = new QGridLayout;
    imagesLayout->setSpacing(6);

    QList<ImageResource>& importedImages = imageResourcesController->getResources();
    int size = importedImages.size();
    for (int i = 0; i < size; i++) {
        addImageButton(importedImages[i]);
    }

    QWidget* verticalSpacer = new QWidget;
    verticalSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Layout

    imagesLayout->addWidget(verticalSpacer);
    scrollWidget->setLayout(imagesLayout);
    scrollArea->setWidget(scrollWidget);

    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(scrollArea);

    widget->setLayout(mainLayout);
    setWidget(widget);

    args.mainWin->addDockWidget(Qt::RightDockWidgetArea, this);

    // Connections

    connect(importBtn, &QPushButton::clicked, this, &ImageResourceImporterDock::importImage);
}

bool ImageResourceImporterDock::isUniqueImage(const QString &imageName)
{
    QList<ImageResource>& importedImages = imageResourcesController->getResources();
    int size = importedImages.size();
    for (int i = 0; i < size; i++) {
        if (importedImages[i].getName() == imageName)
            return false;
    }
    return true;
}

void ImageResourceImporterDock::addImageButton(const ImageResource& currentImage)
{
    const QIcon icon(*currentImage.getPixmap());
    QPushButton* button = new QPushButton();
    button->setIcon(icon);
    button->setIconSize(QSize(70, 70));

    QAction* btnAct = new QAction(currentImage.getName());
    connect(btnAct, &QAction::triggered, this, [&]() {
        QAction* act = qobject_cast<QAction*>(sender());
        QApplication::clipboard()->setText(QString("<img src=\"%1\">").arg(act->text()));
        ((QMainWindow*)window())->statusBar()->showMessage(tr("Image tag copied to clipboard"), 2000);
    });

    button->addAction(btnAct);
    connect(button, &QPushButton::clicked, this, [&]() {
        QPushButton* btn = qobject_cast<QPushButton*>(sender());
        emit btn->actions()[0]->triggered();
    });


    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(button);
    vLayout->addWidget(new QLabel(currentImage.getName()));

    imagesLayout->addLayout(vLayout, (int)row, (int)column);

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

void ImageResourceImporterDock::importImage()
{
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Import Image"), QString(), tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (!filePath.isEmpty()) {
        if (isUniqueImage(QFileInfo(filePath).fileName())) {
            QGuiApplication::setOverrideCursor(Qt::WaitCursor);
            QPixmap* pixmap = new QPixmap(filePath);
            QGuiApplication::restoreOverrideCursor();

            //Add imported image to imagesLayout
            ImageResource newResource = ImageResource(QFileInfo(filePath).fileName(), pixmap, filePath);
            imageResourcesController->appendResource(newResource);
            addImageButton(newResource);
        }
        else {
            QMessageBox::warning(this, QString(), tr("Cannot import images with the same name."));
        }
    }
}
