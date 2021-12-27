#ifndef SAVEFILESTREAM_H
#define SAVEFILESTREAM_H

#include <QSaveFile>
#include <QString>

class MainWindow;

class SaveFileStream
{
public:
    SaveFileStream(QSaveFile& file, const QString& filename, QString& errorMessage, MainWindow* mainWin);

private:
    void saveChocoFile();

private:
    const QString& filename;
    MainWindow* mainWin;
    QSaveFile& file;
};

#endif // SAVEFILESTREAM_H
