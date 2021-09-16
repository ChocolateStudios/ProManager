#ifndef APPLICATION_H
#define APPLICATION_H

class QAxObject;
class QString;

namespace OfficeLib {

class Application
{
public:
    Application(const QString& appName);
    void setVisible(const bool& visible);
    void quit();

protected:
    QAxObject* appObj;
};

}

#endif // APPLICATION_H
