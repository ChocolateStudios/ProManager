#ifndef CUSTOMIMAGEPUSHBUTTON_H
#define CUSTOMIMAGEPUSHBUTTON_H

#include <QPushButton>

class QMainWindow;

class CustomImagePushButton : public QPushButton
{
    Q_OBJECT
public:
    CustomImagePushButton(const QString& imageName, QMainWindow* mainWin);

private:
    QString imageName;
    QMainWindow* mainWin;
};

#endif // CUSTOMIMAGEPUSHBUTTON_H
