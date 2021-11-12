#ifndef CUSTOMFILEPUSHBUTTON_H
#define CUSTOMFILEPUSHBUTTON_H

#include <QPushButton>

class QMainWindow;

class CustomFilePushButton : public QPushButton
{
public:
    CustomFilePushButton(const QString& fileName, QMainWindow* mainWin);

private:
    QString fileName;
    QMainWindow* mainWin;
};

#endif // CUSTOMFILEPUSHBUTTON_H
