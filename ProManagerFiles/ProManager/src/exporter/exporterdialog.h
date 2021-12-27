#ifndef EXPORTERDIALOG_H
#define EXPORTERDIALOG_H

#include "resources/fileexportpropertiesresource.h"

#include "converters/wordconverter.h"
#include "converters/excelconverter.h"
#include "converters/powerpointconverter.h"

#include <QCheckBox>
#include <QDialog>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QFileInfo>
#include <QLabel>
#include <QFileDialog>
#include <QGuiApplication>

class ExporterDialog : public QDialog
{
public:
    ExporterDialog(QList<FileExportPropertiesResource>* resources_, MainWindow* mainWin_)
        : resources(resources_), mainWin(mainWin_)
    {
        // Widgets

        QVBoxLayout* mainLayout = new QVBoxLayout;
        mainLayout->setSizeConstraint(QLayout::SetFixedSize);

        QGroupBox* group = new QGroupBox(QObject::tr("Export Files"));
        QVBoxLayout* groupLayout = new QVBoxLayout;

        int size = resources->size();

        for (int i = 0; i < size; i++) {
            const QString& resourceName = (*resources)[i].getName();
            QCheckBox* resourceCheckBox = new QCheckBox(QString("%1%2").arg(resourceName).arg((*resources)[i].getExtension()));
            groupLayout->addWidget(resourceCheckBox);

            resourcesCheckBoxes.append(resourceCheckBox);
            createPdfOfFileInIndex.append(false);

            if ((*resources)[i].getHasPdfVersion()) {
                QString resourcePdfName = QString("%1%2").arg(resourceName).arg(".pdf");
                QCheckBox* resourcePdfCheckBox = new QCheckBox(resourcePdfName);
                groupLayout->addWidget(resourcePdfCheckBox);

                resourcesCheckBoxes.append(resourcePdfCheckBox);
                createPdfOfFileInIndex[i] = true;
            }

        }

        QHBoxLayout* exportDirectoryLayout = new QHBoxLayout;
        exportDirectoryLabel = new QLabel("Select destination folder");
        exportDirectoryLabel->setFixedWidth(300);

        QPushButton* exportDirectoryBtn = new QPushButton(tr("Browse"));

        QHBoxLayout* optionsLayout = new QHBoxLayout;
        QPushButton* exportBtn = new QPushButton(QObject::tr("Export"));
        QPushButton* cancelBtn = new QPushButton(QObject::tr("Cancel"));


        // Layout

        group->setLayout(groupLayout);
        mainLayout->addWidget(group);

        exportDirectoryLayout->addWidget(exportDirectoryLabel);
        exportDirectoryLayout->addWidget(exportDirectoryBtn);
        mainLayout->addLayout(exportDirectoryLayout);

        optionsLayout->addWidget(exportBtn);
        optionsLayout->addWidget(cancelBtn);
        mainLayout->addLayout(optionsLayout);


        // Connections

        connect(exportDirectoryBtn, &QPushButton::clicked, this, [&]() {
            const QString fileName = QFileDialog::getExistingDirectory(this, tr("Browse"));
            if (!fileName.isEmpty())
                exportDirectoryLabel->setText(fileName);
        });


        connect(exportBtn, &QPushButton::clicked, this, [&]() {
            if (exportDirectoryLabel->text() == "Select destination folder") return;

            int resSize = resources->size();

            QGuiApplication::setOverrideCursor(Qt::WaitCursor);

            for (int i = 0, checkBoxIndex = 0; i < resSize; i++, checkBoxIndex++) {
                const FileExportPropertiesResource& res = (*resources)[i];
                BaseConverter* converter = nullptr;

                if (res.getExtension() == ".docx") {
                    converter = new WordConverter(mainWin, res, exportDirectoryLabel->text());
                }
                else if (res.getExtension() == ".xlsx") {
                    converter = new ExcelConverter(mainWin, res, exportDirectoryLabel->text());
                }
                else if (res.getExtension() == ".pptx") {
                    converter = new PowerPointConverter(mainWin, res, exportDirectoryLabel->text());
                }

                if (converter) {

                    if (resourcesCheckBoxes[checkBoxIndex]->isChecked()) {
                        converter->exportTo();
                    }

                    if (createPdfOfFileInIndex[i]) {
                        if (resourcesCheckBoxes[checkBoxIndex + 1]->isChecked())
                            converter->exportToPdf();
                        checkBoxIndex++;
                    }

                    delete converter;
                }
            }

            QGuiApplication::restoreOverrideCursor();

            this->close();
        });


        connect(cancelBtn, &QPushButton::clicked, this, [&]() {
            qDebug() << exportDirectoryLabel->text();
            this->close();
        });

        setLayout(mainLayout);
    }

private:
    QList<FileExportPropertiesResource>* resources;
    MainWindow* mainWin;

    QList<bool> createPdfOfFileInIndex;
    QList<QCheckBox*> resourcesCheckBoxes;
    QLabel* exportDirectoryLabel;
};

#endif // EXPORTERDIALOG_H
