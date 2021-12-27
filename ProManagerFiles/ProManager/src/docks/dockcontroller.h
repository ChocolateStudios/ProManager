#ifndef DOCKCONTROLLER_H
#define DOCKCONTROLLER_H

#include <QIcon>
#include <QAction>

class ResourcesManager;
class DockFactory;
class BaseDock;
class MainWindow;
class StylesContextMenu;
class TextAnalizer;

class DockController : public QObject
{
    Q_OBJECT
public:
    DockController(MainWindow* mainWin);

    bool isDockVisible(const unsigned int& id);
    QAction* getAddStyleAction();

private:
    void createActions();

signals:
    void dockById(int);

public slots:
    void createOrDestroyDockById(int id);

private:
    QList<BaseDock*> docks;
    QList<QAction*> toggleDockActions;

    DockFactory* dockFactory;
    int dockTypes;
};

#endif // DOCKCONTROLLER_H
