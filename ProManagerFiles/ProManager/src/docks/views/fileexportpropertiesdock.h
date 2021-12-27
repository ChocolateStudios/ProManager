#ifndef FILEEXPORTPROPERTIESDOCK_H
#define FILEEXPORTPROPERTIESDOCK_H

#include "docks/basedock.h"

#include "resources/fileexportpropertiesresource.h"

class QVBoxLayout;
class PopupButton;
class FileResource;
class FileExportPropertiesResourcesController;

struct DockArguments;

template<typename T>
class ResourcesController;

class FileExportPropertiesDock : public BaseDock
{
    Q_OBJECT
    friend class DockFactory;

public:
    FileExportPropertiesDock(DockArguments args);

private slots:
    void addSectionToBlockChain();

private:
    void addSections(const QStringList& sections);
    void widgetsVisible(const bool& visible);

private:
    FileExportPropertiesResourcesController* fileExportPropertiesController;
    const QStringList* tabNames;
    QList<FileResource>* fileResources;

    FileExportPropertiesResource fileExportPropertiesCreated;
    QList<PopupButton*> sectionsPopupBtns;

    QVBoxLayout* blockChainLayout;
    QWidget* scrollWidget;
    PopupButton* selectSectionBtn;
};

#endif // FILEEXPORTPROPERTIESDOCK_H
