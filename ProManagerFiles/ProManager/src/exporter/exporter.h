#ifndef EXPORTER_H
#define EXPORTER_H

class MainWindow;

class Exporter
{
public:
    Exporter(MainWindow* mainWindow);

private:
    void showDownloadDialog();

private:
    MainWindow* mainWin;
};

#endif // EXPORTER_H
