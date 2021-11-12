#ifndef NAVEGATIONCONFIGDOCK_H
#define NAVEGATIONCONFIGDOCK_H

#include <QDockWidget>
#include <QDebug>

#include <extras/style.h>

class QStandardItemModel;

class NavegationConfigDock : QDockWidget
{
    Q_OBJECT
    friend class NavegationDock;
private:
    NavegationConfigDock(QMainWindow *mWin, QDockWidget* navDck, const QList<Style> styles);
    //~NavegationConfigDock() { qDebug() << "Muerto D:"; }
    const QStandardItemModel* getModel() const;

    void closeEvent(QCloseEvent* event) override;

    QMainWindow *mainWin;
    QDockWidget* navDock;
    QStandardItemModel*  model;
};

#endif // NAVEGATIONCONFIGDOCK_H
