#ifndef STYLERESOURCESCONTROLLER_H
#define STYLERESOURCESCONTROLLER_H

#include "menus/stylescontextmenu.h"

#include "resources/controllers/resourcescontroller.h"
#include "resources/styleresource.h"

#include "observers/subjects/subject.h"

class StyleResourcesController : public ResourcesController<StyleResource>, public Subject
{
public:
    StyleResourcesController() {
        StyleResource commentStyle = StyleResource(QObject::tr("Comment"), Qt::white);
        appendResource(commentStyle);
    }

    StyleResource* getStyleByName(const QString& styleName) {
        int size = resources.size();
        for(int i = 0; i < size; i++) {
            if (resources[i].getName() == styleName)
                return &resources[i];
        }
        return nullptr;
    }

    StyleResource* getStyleByIconColor(const QColor& styleIconColor) {
        int size = resources.size();
        for(int i = 0; i < size; i++) {
            if (resources[i].getIconColor() == styleIconColor)
                return &resources[i];
        }
        return nullptr;
    }

    inline const QString& getCommentStyleName() {
        return resources[0].getName();
    }

    inline void readResources(QDataStream& in, StylesContextMenu* menu) override {
        in >> resources;

        menu->clearStyles();
        int size = resources.size();
        for (int i = 0; i < size; i++)
            menu->addStyle(resources[i]);
    }
};

#endif // STYLERESOURCESCONTROLLER_H
