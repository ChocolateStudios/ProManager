#include "customimagepushbutton.h"

#include <QApplication>
#include <QClipboard>
#include <QMainWindow>
#include <QStatusBar>

CustomImagePushButton::CustomImagePushButton(const QString& imgName, QMainWindow* win)
    : imageName(imgName), mainWin(win)
{
    connect(this, &QPushButton::clicked, this, [&]() {
        QApplication::clipboard()->setText(QString("<img src=\"%1\">").arg(imageName));
        mainWin->statusBar()->showMessage(tr("Image tag copied to clipboard"), 2000);
    });
}
