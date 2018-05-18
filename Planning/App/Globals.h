#ifndef GLOBALS
#define GLOBALS

#include <array>
#include <QString>
#include <QMessageBox>
#include <QSplashScreen>
#include <QApplication>
#include <QProgressBar>

namespace Globals
{
    static const QString messageBoxStyleSheet("QMessageBox {background-color: rgb(230, 240, 249);}"
                                              "QLabel{font: 14px \"Open Sans\";color: rgb(88,88,90);}"

                                              "QPushButton { background-color:  qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                                             "stop: 0 	    #FF007F96, stop: 0.614 white,"
                                                             "stop: 0.614   #7aacd7,  stop: 0.853 white,"
                                                             "stop: 0.853 	#FF007F96, stop: 1.0 #FF007F96);"
                                                             "border-style: none;border-radius: 5px;"
                                                             "color: white; min-height: 2em; min-width: 6em;"
                                                             "font: 10pt \"Open Sans\";}"
                                              "QPushButton::hover {background-color:  qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                                             "stop: 0  #c60c30,  stop: 0.614 white,"
                                                             "stop: 0.614   #e5506c, stop: 0.853 white,"
                                                             "stop: 0.853 	#c60c30,  stop: 1.0 #c60c30);"
                                                             "border-style: none;border-radius: 5px;"
                                                             "color: white;min-height: 2em;min-width: 6em;"
                                                             "font: 10pt \"Open Sans\";}"
                                              "QPushButton::disabled {background-color: rgb(155, 155, 155);"
                                                             "border-style: none;border-radius: 5px;"
                                                             "color: white;min-height: 2em;min-width: 6em;"
                                                             "font: 10pt \"Open Sans\";}");

    static void showMessage(const QMessageBox::Icon& icon,const QString& tittle, const QString& text)
    {
        QMessageBox *message = new QMessageBox(icon, tittle, text);
        message->setAttribute(Qt::WA_DeleteOnClose);
        message->setStyleSheet(messageBoxStyleSheet);
        message->show();
    }

    static QColor getColorForPhase(const int number){
        switch(number % 7){
            case 0:
                return QColor("#fb9a99");
            case 1:
                return QColor("#209fdf");
            case 2:
                return QColor("#ffa500");
            case 3:
                return QColor("#ed1004");
            case 4:
                return QColor("#ffff00");
            case 5:
                return QColor("#90ee90");
            case 6:
                return QColor("#b15928");
            default:
                return QColor("#000000");
        }
    }
}

#endif // GLOBALS

