#ifndef READFILESTREAM_H
#define READFILESTREAM_H

#include <QString>

class MainWindow;
class QFile;

class ReadFileStream
{
public:
    ReadFileStream(QFile& file, const QString& filename, MainWindow* mainWin);

private:
    void readChocoFile();

private:
    QFile& file;
    const QString& filename;
    MainWindow* mainWin;
};

#endif // READFILESTREAM_H
