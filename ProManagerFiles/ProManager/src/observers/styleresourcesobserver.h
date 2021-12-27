#ifndef STYLERESOURCESOBSERVER_H
#define STYLERESOURCESOBSERVER_H

#include "observers/observer.h"

#include <QList>

class StyleResource;

class StyleResourcesObserver : public Observer
{
public:
    StyleResourcesObserver(QList<StyleResource>* _styles)
        : styles(_styles)
    {
    }

protected:
    QList<StyleResource>* styles;
};

#endif // STYLERESOURCESOBSERVER_H
