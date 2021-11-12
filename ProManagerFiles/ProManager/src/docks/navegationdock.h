#ifndef NAVEGATIONDOCK_H
#define NAVEGATIONDOCK_H

#include "docks/basedock.h"

#include <extras/style.h>

class NavegationConfigDock;
class QStandardItemModel;
class QTreeView;
class CustomTextEdit;

class NavegationDock : public BaseDock
{
    Q_OBJECT
    friend class DockController;

private:
    NavegationDock(QMainWindow *mainWin, const QList<Style>* stls, const QString& docTitle, CustomTextEdit* txtEdit);
    ~NavegationDock();

private slots:
    void update();

private:
    NavegationConfigDock* navConfigDock;
    const QString& documentTitle;
    CustomTextEdit* textEdit;

    QStandardItemModel* model = nullptr;
    QTreeView* treeView = nullptr;
};

#endif // NAVEGATIONDOCK_H
