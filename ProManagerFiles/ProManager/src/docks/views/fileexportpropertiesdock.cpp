#include "fileexportpropertiesdock.h"

#include "docks/dockarguments.h"

#include "resources/controllers/fileexportpropertiesresourcescontroller.h"
#include "resources/fileresource.h"

#include "widgets/popupbutton.h"

#include <QAction>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

FileExportPropertiesDock::FileExportPropertiesDock(DockArguments args)
    : BaseDock(args.mainWin, tr("File Export Properties")), fileExportPropertiesController(args.get<FileExportPropertiesResourcesController>(0))
        , tabNames(args.get<QStringList>(1)), fileResources(args.get<QList<FileResource>>(2))
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Widgets

    QWidget* widget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* selectExportableFileLayout = new QHBoxLayout;
    PopupButton* selectExportableFileBtn = new PopupButton("Select file");
    QPushButton* createExportableFileBtn = new QPushButton(QIcon(":/images/dock-0"), "Create new");

    QHBoxLayout* fileNameLayout = new QHBoxLayout;
    QLineEdit* fileNameLineEdit = new QLineEdit("newDocument");
    PopupButton* selectFileExtensionBtn = new PopupButton("Select extension");

    QHBoxLayout* templateLayout = new QHBoxLayout;
    QCheckBox* useTemplateCheckBox = new QCheckBox("Use template");
    PopupButton* selectTemplateBtn = new PopupButton("Select template");

    QCheckBox* hasPdfVersionCheckBox = new QCheckBox("Include pdf version");

    QVBoxLayout* sectionsLayout = new QVBoxLayout;
    QLabel* sectionsLabel = new QLabel("Sections");

    QScrollArea* scrollArea = new QScrollArea;
    scrollWidget = new QWidget;

    blockChainLayout = new QVBoxLayout;
    selectSectionBtn = new PopupButton("+", 100);

    QPushButton* applyBtn = new QPushButton("Apply");

    QWidget* verticalSpacer = new QWidget;
    verticalSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Layout

    selectExportableFileLayout->addWidget(selectExportableFileBtn);
    selectExportableFileLayout->addWidget(createExportableFileBtn);
    mainLayout->addLayout(selectExportableFileLayout);

    fileNameLayout->addWidget(fileNameLineEdit);
    fileNameLayout->addWidget(selectFileExtensionBtn);
    mainLayout->addLayout(fileNameLayout);

    templateLayout->addWidget(useTemplateCheckBox);
    templateLayout->addWidget(selectTemplateBtn);
    mainLayout->addLayout(templateLayout);

    mainLayout->addWidget(hasPdfVersionCheckBox);

    sectionsLayout->addWidget(sectionsLabel);

    blockChainLayout->addWidget(selectSectionBtn);
    scrollWidget->setLayout(blockChainLayout);
    scrollArea->setWidget(scrollWidget);

    sectionsLayout->addWidget(scrollArea);

    mainLayout->addLayout(sectionsLayout);

    mainLayout->addWidget(applyBtn);

    mainLayout->addWidget(verticalSpacer);
    widget->setLayout(mainLayout);
    setWidget(widget);

    args.mainWin->addDockWidget(Qt::RightDockWidgetArea, this);

    widgetsVisible(false);
    selectTemplateBtn->setVisible(false);

    // Connections

    QList<FileExportPropertiesResource>& resources = fileExportPropertiesController->getResources();
    int size = resources.size();
    for (int i = 0; i < size; i++) {
        const FileExportPropertiesResource& res = resources[i];

        QAction* newAction = new QAction(QString("%1%2").arg(res.getName(), res.getExtension()));
        connect(newAction, &QAction::triggered, this, [&]() {
            QAction* act = qobject_cast<QAction*>(sender());
            const FileExportPropertiesResource* currentResource =
                    fileExportPropertiesController->getFileExportPropertiesByNameAndExtension(act->text());

            if (!currentResource) return;

            ((QLineEdit*)this->widget()->children()[3])->setText(currentResource->getName());

            PopupButton* fileExtBtn = ((PopupButton*)this->widget()->children()[4]);
            const QList<QAction*>& fileExtActs = fileExtBtn->getMenuActions();
            for (int i = 0; i < 3; i++) {
                if (fileExtActs[i]->text() == currentResource->getExtension())
                    emit fileExtActs[i]->triggered();
            }

            ((QCheckBox*)this->widget()->children()[5])->setChecked(!currentResource->getTemplateFile().isEmpty());
            PopupButton* templateBtn = ((PopupButton*)this->widget()->children()[6]);
            const QList<QAction*>& templateActs = templateBtn->getMenuActions();
            int templateActsSize = templateActs.size();
            for (int i = 0; i < templateActsSize; i++) {
                if (templateActs[i]->text() == currentResource->getTemplateFile())
                    emit templateActs[i]->triggered();
            }

            ((QCheckBox*)this->widget()->children()[7])->setChecked(currentResource->getHasPdfVersion());

            addSections(currentResource->getSectionsBlockChain());
            widgetsVisible(true);
        });
        selectExportableFileBtn->addActionToMenu(newAction);
    }


    connect(createExportableFileBtn, &QPushButton::clicked, this, [&]() {
        ((QLineEdit*)this->widget()->children()[3])->setText("newDocument");

        ((PopupButton*)this->widget()->children()[4])->setText("Select extension");

        ((QCheckBox*)this->widget()->children()[5])->setChecked(false);
        ((QCheckBox*)this->widget()->children()[7])->setChecked(false);

        // TODO: Clear Sections BlockChain
        widgetsVisible(true);
    });


    QAction* wordExtension = new QAction(QIcon(":/images/word_icon.png"), ".docx");
    QAction* excelExtension = new QAction(QIcon(":/images/excel_icon.png"),".xlsx");
    QAction* pptExtension = new QAction(QIcon(":/images/ppt_icon.png"),".pptx");
    selectFileExtensionBtn->addActionToMenu(wordExtension);
    selectFileExtensionBtn->addActionToMenu(excelExtension);
    selectFileExtensionBtn->addActionToMenu(pptExtension);


    connect(useTemplateCheckBox, &QCheckBox::clicked, this, [&](bool checked) {
        ((PopupButton*)this->widget()->children()[6])->setVisible(checked);
    });

    size = fileResources->size();
    for (int i = 0; i < size; i++) {
        QAction* newAction = new QAction((*fileResources)[i].getName());
        connect(newAction, &QAction::triggered, this, [&]() {
            QAction* act = qobject_cast<QAction*>(sender());
            fileExportPropertiesCreated.setTemplateFile(act->text());
        });
        selectTemplateBtn->addActionToMenu(newAction);
    }

    size = tabNames->size();
    for (int i = 0; i < size; i++) {
        QAction* newAction = new QAction(tabNames->at(i));
        connect(newAction, &QAction::triggered, this, &FileExportPropertiesDock::addSectionToBlockChain);
        selectSectionBtn->addActionToMenu(newAction);
    }


    connect(applyBtn, &QPushButton::clicked, this, [&]() {
        const QString& extension = ((PopupButton*)(this->widget()->children()[4]))->text();
        if (extension == "Select extension")
            return;

        const QString& exportFileName = ((QLineEdit*)(this->widget()->children()[3]))->text();
        if (exportFileName.isEmpty())
            return;

        const bool& hasPdfVersion = ((QCheckBox*)(this->widget()->children()[7]))->isChecked();

        fileExportPropertiesCreated.setName(exportFileName);
        fileExportPropertiesCreated.setExtension(extension);
        fileExportPropertiesCreated.setHasPdfVersion(hasPdfVersion);

        fileExportPropertiesController->appendResource(fileExportPropertiesCreated);

        delete this;
    });

    // To see index of child objects
//    qDebug() << "";

//    auto printArray = [&](const QObjectList& list) {
//        for (int i = 0; i < list.size(); i++)
//            qDebug() << QString("%1:").arg(i) << list[i];
//        qDebug() << "";
//    };

//    printArray(this->widget()->children());
}

void FileExportPropertiesDock::addSectionToBlockChain()
{
    QAction* act = qobject_cast<QAction*>(sender());
    fileExportPropertiesCreated.addSection(act->text());

    PopupButton* newSelectSectionBtn = new PopupButton("+", 100);
    int tabsSize = tabNames->size();
    for (int j = 0; j < tabsSize; j++) {
        QAction* newAct = new QAction(tabNames->at(j));
        connect(newAct, &QAction::triggered, this, &FileExportPropertiesDock::addSectionToBlockChain);
        newSelectSectionBtn->addActionToMenu(newAct);
    }

    blockChainLayout->addWidget(newSelectSectionBtn);
    sectionsPopupBtns.append(newSelectSectionBtn);

    QRect scrollRect = scrollWidget->geometry();
    scrollRect.setHeight(scrollRect.height() + 40);
    scrollWidget->setGeometry(scrollRect);
}

void FileExportPropertiesDock::addSections(const QStringList &sections)
{
    int sectionsSize = sections.size();
    if (sectionsSize > 0) {
        auto addSectionFunc = [&](PopupButton* btn, int& sectionIndex) {
            if (sectionIndex >= sections.size()) return false;

            const QList<QAction*>& btnActs = btn->getMenuActions();
            int actsSize = btnActs.size();
            for (int i = 0; i < actsSize; i++) {
                if (btnActs[i]->text() == sections[sectionIndex]) {
                    emit btnActs[i]->triggered();
                    sectionIndex++;
                    return true;
                }
            }
            return false;
        };

        int sectionIndex = 0;
        if (addSectionFunc(selectSectionBtn, sectionIndex)) {
            for (int i = 0; i < sectionsPopupBtns.size(); i++) {
                if (!addSectionFunc(sectionsPopupBtns[i], sectionIndex))
                    break;
            }
        }
    }
}

void FileExportPropertiesDock::widgetsVisible(const bool &visible)
{
    ((QLineEdit*)this->widget()->children()[3])->setVisible(visible); //fileNameLineEdit
    ((PopupButton*)this->widget()->children()[4])->setVisible(visible); //selectFileExtensionBtn
    ((QCheckBox*)this->widget()->children()[5])->setVisible(visible); // useTemplateCheckBox
//    ((PopupButton*)this->widget()->children()[6])->setVisible(visible); // selectTemplateBtn
    ((QCheckBox*)this->widget()->children()[7])->setVisible(visible); // hasPdfVersionCheckBox
    ((QLabel*)this->widget()->children()[8])->setVisible(visible); // sectionsLabel
    ((QScrollArea*)this->widget()->children()[9])->setVisible(visible); // scrollArea
    ((QPushButton*)this->widget()->children()[10])->setVisible(visible); //applyBtn
}
