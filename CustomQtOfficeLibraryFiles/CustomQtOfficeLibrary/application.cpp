#include "application.h"

#include <QString>
#include <QAxObject>

namespace OfficeLib {

Application::Application(const QString &appName) : appObj(new QAxObject(appName))
{
}

void Application::setVisible(const bool &visible)
{
    appObj->setProperty("Visible", visible);
}

void Application::quit()
{
    appObj->dynamicCall("Quit()");
}

}
