#include "customfilepushbutton.h"

#include <QApplication>
#include <QClipboard>
#include <QMainWindow>
#include <QStatusBar>

CustomFilePushButton::CustomFilePushButton(const QString &fName, QMainWindow *win)
    : fileName(fName), mainWin(win)
{
    connect(this, &QPushButton::clicked, this, [&]() {
        QApplication::clipboard()->setText(QString("<file src=\"%1\">").arg(fileName));
        mainWin->statusBar()->showMessage(tr("File tag copied to clipboard"), 2000);
    });
}
