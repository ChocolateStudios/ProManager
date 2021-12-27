#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    bool splashFinished = false;
    QSplashScreen splash;
    splash.setPixmap(QPixmap(":images/splash-01.png"));
    splash.show();

    splash.showMessage(QObject::tr("Setting up the main window"), Qt::AlignRight | Qt::AlignTop, Qt::black);

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

        if (!splashFinished) {
            splash.showMessage(QObject::tr("Loading modules..."), Qt::AlignRight | Qt::AlignTop, Qt::white);
        }

        newWin->tile(mainWin);
        newWin->show();
        mainWin = newWin;

        if (!splashFinished) {
            splash.finish(newWin);
            splashFinished = true;
        }
    }

    if (!mainWin) {
        mainWin = new MainWindow;

        if (!splashFinished) {
            splash.showMessage(QObject::tr("Loading modules..."), Qt::AlignRight | Qt::AlignTop, Qt::white);
        }
    }

    mainWin->show();

    if (!splashFinished) {
        splash.finish(mainWin);
        splashFinished = true;
    }

    return app.exec();
}
