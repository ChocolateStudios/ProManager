#ifndef FILEEXPORTPROPERTIESRESOURCE_H
#define FILEEXPORTPROPERTIESRESOURCE_H

#include "resource.h"

#include <QStringList>

class FileExportPropertiesResource : public Resource
{
    friend QDataStream &operator>>(QDataStream& in, FileExportPropertiesResource& resource);
    friend QDataStream &operator<<(QDataStream& out, FileExportPropertiesResource resource);

public:
    FileExportPropertiesResource()
        : Resource()
    {
    }

    FileExportPropertiesResource(const QString& newName)
        : Resource(newName)
    {
    }

    inline const QStringList &getSectionsBlockChain() const { return sectionsBlockChain; }
    inline void setSectionsBlockChain(const QStringList& newSectionsBlockChain) { sectionsBlockChain = newSectionsBlockChain; }

    inline const bool &getHasPdfVersion() const { return hasPdfVersion; }
    inline void setHasPdfVersion(const bool& value) { hasPdfVersion = value; }

    inline const QString &getExtension() const { return extension; }
    inline void setExtension(const QString& newExtension) { extension = newExtension; }

    inline const QString &getTemplateFile() const { return templateFile; }
    inline void setTemplateFile(const QString& newTemplateFile) { templateFile = newTemplateFile; }


    inline void addSection(const QString& sectionName) { sectionsBlockChain.append(sectionName); }
    inline void updateSection(const unsigned int& index, const QString& newSectionName) { sectionsBlockChain[index] = newSectionName; }
    void updateSectionPosition(const unsigned int& oldPosition, const unsigned int& newPosition) {
        const QString& section = sectionsBlockChain[oldPosition];
        sectionsBlockChain.remove(oldPosition);
        sectionsBlockChain.insert(newPosition, section);
    }

    inline void removeSection(const unsigned int& index) { sectionsBlockChain.remove(index); }


private:
    QStringList sectionsBlockChain;
    bool hasPdfVersion = false;
    QString extension;
    QString templateFile;
};

#endif // FILEEXPORTPROPERTIESRESOURCE_H
