#include "resources/fileexportpropertiesresource.h"

#include <QDataStream>

QDataStream &operator>>(QDataStream& in, FileExportPropertiesResource& resource)
{
    QString name;
    QStringList sectionsBlockChain;
    bool hasPdfVersion;
    QString extension;
    QString templateFile;

    in >> name >> sectionsBlockChain >> hasPdfVersion >> extension >> templateFile;
    resource.setName(name);
    resource.setSectionsBlockChain(sectionsBlockChain);
    resource.setHasPdfVersion(hasPdfVersion);
    resource.setExtension(extension);
    resource.setTemplateFile(templateFile);
    return in;
}

QDataStream &operator<<(QDataStream& out, FileExportPropertiesResource resource)
{
    out << resource.getName() << resource.getSectionsBlockChain() << resource.getHasPdfVersion()
        << resource.getExtension() << resource.getTemplateFile();
    return out;
}
