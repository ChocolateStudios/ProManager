#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Chocolate Studios");
    QCoreApplication::setApplicationName("ProManager");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow *mainWin = nullptr;
    const QStringList posArgs = parser.positionalArguments();
    for (const QString& file : posArgs) {
        MainWindow* newWin = new MainWindow(file);
        newWin->tile(mainWin);
        newWin->show();
        mainWin = newWin;
    }

    if (!mainWin)
        mainWin = new MainWindow;

    mainWin->show();
    return app.exec();
}
