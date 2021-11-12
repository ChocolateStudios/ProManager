#ifndef BASEDOCK_H
#define BASEDOCK_H

#include <QDockWidget>
#include <QMainWindow>
#include <QDebug>

class BaseDock : public QDockWidget
{
    friend class DockController;

protected:
    BaseDock(const QString& dockName, QMainWindow* mainWin)
        : QDockWidget(dockName, mainWin)
    {
    }
    //~BaseDock() { qDebug() << "Muerto D:"; }

    void closeEvent(QCloseEvent* event) override
    {
        QDockWidget::closeEvent(event);
        autoDestroy();
    }

    inline void autoDestroy()
    {
        delete this;
    }
};

#endif // BASEDOCK_H
