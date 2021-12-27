#ifndef BASEDOCK_H
#define BASEDOCK_H

#include "observers/observer.h"

#include <QDockWidget>
#include <QMainWindow>
#include <QDebug>

class BaseDock : public QDockWidget
{
public:
    BaseDock(QMainWindow* mainWin, const QString& dockName)
        : QDockWidget(dockName, mainWin)
    {
    }
    virtual ~BaseDock() { /*qDebug() << "Muerto D:";*/ }

protected:
    void closeEvent(QCloseEvent* event) override
    {
        QDockWidget::closeEvent(event);
        delete this;
    }
};

#endif // BASEDOCK_H
