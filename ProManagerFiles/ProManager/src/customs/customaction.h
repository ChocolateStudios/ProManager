#ifndef CUSTOMACTION_H
#define CUSTOMACTION_H

#include "extras/style.h"

#include <QAction>

class Style;

class CustomAction : public QAction
{
    Q_OBJECT
public:
    CustomAction(QObject *parent = nullptr);
    CustomAction(const Style style, QObject* parent = nullptr);

    const Style &getStyle() const;
    void setStyle(const Style newStyle);

private:
    Style* style;
};

#endif // CUSTOMACTION_H
