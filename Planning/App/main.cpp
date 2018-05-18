#include "MainWindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    const static int SizeFactor = qMin(QApplication::desktop()->geometry().width(), QApplication::desktop()->geometry().height());
    const static int HeightFactor = 60;
    const static int WidthFactor = 64;
    const static int Percent = 100;
    static QSize SplashSize(SizeFactor * WidthFactor/ Percent, SizeFactor * HeightFactor/ Percent);
    QSplashScreen *splash = new QSplashScreen(QPixmap(":/Resources/appIcon.png").scaled(SplashSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    splash->setAttribute(Qt::WA_DeleteOnClose);
    const int SPLASH_DURATION = 5000;
    const int SPLASH_INTERVAL = 200;
    splash->show();
    QTimer::singleShot(SPLASH_DURATION, [splash](){splash->close();QApplication::processEvents();});
    QTimer::singleShot(SPLASH_DURATION + SPLASH_INTERVAL, [](){
        MainWindow *w = new MainWindow();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->showMaximized();});
    return a.exec();
}
