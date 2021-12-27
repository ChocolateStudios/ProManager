#ifndef RESOURCE_H
#define RESOURCE_H

#include <QString>

class Resource
{
public:
    Resource()
    {
    }

    Resource(const QString& newName)
        : name(newName)
    {
    }

    inline const QString &getName() const { return name; }
    inline void setName(const QString& newName) { name = newName; }

protected:
    QString name;
};

#endif // RESOURCE_H
