#ifndef RESOURCESCONTROLLER_H
#define RESOURCESCONTROLLER_H

#include <QList>
#include <QDebug>

class StylesContextMenu;

class BaseResourcesController
{
public:
    virtual inline void readResources(QDataStream& in, StylesContextMenu* menu) = 0;
    virtual inline void saveResources(QDataStream& out) = 0;
};

template<typename T>
class ResourcesController : public BaseResourcesController
{
public:
    inline void appendResource(T resource) {
        resources.append(resource);
    }
    inline void insertResource(const unsigned int& index, T resource) {
        resources.insert(index, resource);
    }
    inline void updateResource(const unsigned int& index, T newResource) {
        resources.replace(index, newResource);
    }
    inline void removeResource(const unsigned int& index) {
        resources.remove(index);
    }
    inline QList<T>& getResources() {
        return resources;
    }
    inline const T& getResource(const unsigned int& index) {
        return resources.at(index);
    }
    virtual inline void readResources(QDataStream& in, StylesContextMenu* menu) override {
        in >> resources;
    }
    inline void saveResources(QDataStream& out) override {
        out << resources;
    }
    T* getResourceByName(const QString& resourceName) {
        int size = resources.size();
        for(int i = 0; i < size; i++) {
            if (resources[i].getName() == resourceName)
                return &resources[i];
        }
        return nullptr;
    }

protected:
    QList<T> resources;
};

#endif // RESOURCESCONTROLLER_H
