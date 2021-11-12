#include "imagesdock.h"

#include "customs/customimagepushbutton.h"
#include "customs/customimage.h"

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

ImagesDock::ImagesDock(QMainWindow *mainWin, QList<CustomImage>* importedImgs)
    :BaseDock(tr("Images"), mainWin), row(0), column(0), importedImages(importedImgs)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* widget = new QWidget;

    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* headerLayout = new QHBoxLayout;
    QPushButton* importBtn = new QPushButton("Import");
    QPushButton* removeBtn = new QPushButton("Remove");
    headerLayout->addWidget(importBtn);
    headerLayout->addWidget(removeBtn);

    connect(importBtn, &QPushButton::clicked, this, &ImagesDock::importImage);

    QScrollArea* scrollArea = new QScrollArea;
    scrollWidget = new QWidget;

    QRect rect = scrollWidget->geometry();
    rect.setHeight(0);
    rect.setWidth(188);
    scrollWidget->setGeometry(rect);

    imagesLayout = new QGridLayout;
    imagesLayout->setSpacing(6);
    for(const CustomImage& cImage : *importedImages) {
        addImageButton(cImage);
    }

    QWidget* verticalSpacer = new QWidget;
    verticalSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    imagesLayout->addWidget(verticalSpacer);
    scrollWidget->setLayout(imagesLayout);
    scrollArea->setWidget(scrollWidget);

    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(scrollArea);

    widget->setLayout(mainLayout);
    setWidget(widget);

    mainWin->addDockWidget(Qt::RightDockWidgetArea, this);
}

bool ImagesDock::isUniqueImage(const QString &imageName)
{
    for (const CustomImage& c : *importedImages) {
        if (c.name == imageName)
            return false;
    }
    return true;
}

void ImagesDock::addImageButton(const CustomImage& currentImage)
{
    const QIcon icon(*currentImage.pixmap);
    CustomImagePushButton* button = new CustomImagePushButton(currentImage.name, (QMainWindow*)window());
    button->setIcon(icon);
    button->setIconSize(QSize(70, 70));


    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(button);
    vLayout->addWidget(new QLabel(currentImage.name));

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

void ImagesDock::importImage()
{
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Import Image"), QString(), tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (!filePath.isEmpty()) {
        if (isUniqueImage(QFileInfo(filePath).fileName())) {
            QGuiApplication::setOverrideCursor(Qt::WaitCursor);
            QPixmap* pixmap = new QPixmap(filePath);
            QGuiApplication::restoreOverrideCursor();

            //Add imported image to imagesLayout
            CustomImage cImg(pixmap, filePath);
            importedImages->append(cImg);
            addImageButton(cImg);
        }
        else {
            QMessageBox::warning(this, QString(), tr("Cannot import images with the same name."));
        }
    }
}
